#include "esp.h"
#include "../../hook/panel/panels.h"
#include "../../tools/draw/cdrawmanager.h"
#include "../../tools/util/util.h"
namespace ESP {
  void Run( CBaseEntity* pLocal ) {

    if( !Global.ESP_enable.value ) {
      return;
    }

    if( Global.ESP_fov.value || Global.ESP_around_fov.value ) {
      float cx = (float)gScreen.Width / 2.0f;
      float cy = (float)gScreen.Height / 2.0f;

      int radius = tanf( DEG2RAD( Global.Aimbot_fov.value ) / 2 ) / tanf( DEG2RAD( ( pLocal->GetCond() & tf_cond::TFCond_Zoomed && !Global.sniper_nozoom.value ) ? 30.0f : 90.0f ) / 2 ) * gScreen.Width;

      if( Global.ESP_fov.value == 1 ) {
        DrawManager::DrawCircle( cx, cy, radius, 16.0f, Global.color_fov.get_color() );
      } else if( Global.ESP_fov.value == 2 ) {
        DrawManager::Rotating_Dot_Circle( cx, cy, radius, 14.0f, Global.color_fov.get_color() );
      }

      if( Global.ESP_around_fov.value ) {
        for( int i = 1; i <= Int::Engine->GetMaxClients(); i++ ) {
          if( i == me ) {
            continue;
          }

          CBaseEntity* pEntity = GetBaseEntity( i );

          if( !pEntity ) {
            continue;
          }

          if( pEntity->IsDormant() ) {
            continue;
          }

          if( pEntity->GetLifeState() != LIFE_ALIVE ) {
            continue;
          }

          static ConVar* mp_friendlyfire = Int::cvar->FindVar( "mp_friendlyfire" );

          if( mp_friendlyfire->GetInt() == 0 && pEntity->GetTeamNum() == pLocal->GetTeamNum() ) {
            continue;
          }

          Vector angle = Util::CalcAngle( pLocal->GetVecOrigin(), pEntity->GetVecOrigin() );
          Vector view_angle;
          Int::Engine->GetViewAngles( view_angle );
          const float deg = Util::GetClockwiseAngle( view_angle, angle );
          //rotation
          const float xrot = cos( deg - PI / 2 );
          const float yrot = sin( deg - PI / 2 );
          //start and end
          const float x1 = ( radius + 5.0f ) * xrot;
          const float y1 = ( radius + 5.0f ) * yrot;
          const float x2 = ( radius + 15.0f ) * xrot;
          const float y2 = ( radius + 15.0f ) * yrot;
          //arrow constants
          constexpr float arrow_angle = DEG2RAD( 90.0f );
          constexpr float arrow_lenght = 6.0f;
          //arrow math
          const Vector line{ x2 - x1, y2 - y1, 0.0f };
          const float length = line.Length();
          //base of arrow
          const float fpoint_on_line = arrow_lenght / ( atanf( arrow_angle ) * length );
          const Vector point_on_line = Vector( x2, y2, 0 ) + ( line * fpoint_on_line * -1.0f );
          const Vector normal_vector{ -line.y, line.x, 0.0f };
          const Vector normal = Vector( arrow_lenght, arrow_lenght, 0.0f ) / ( length * 2 );
          //left and right points
          const Vector rotation = normal * normal_vector;
          const Vector left = point_on_line + rotation;
          const Vector right = point_on_line - rotation;
          //arrow
          //  ^
          //  |
          DrawManager::DrawLine( cx + x2, cy + y2, cx + left.x, cy + left.y, Global.color_fov.get_color() );
          DrawManager::DrawLine( cx + x2, cy + y2, cx + right.x, cy + right.y, Global.color_fov.get_color() );
          DrawManager::DrawLine( cx + x1, cy + y1, cx + x2, cy + y2, Global.color_fov.get_color() );
        }
      }
    }

    if( ( Global.ESP_target.value == 1 || Global.ESP_target.value == 3 ) && Global.aim_index != -1 && Global.Aimbot_auto_aim.KeyDown() ) {
      Vector draw0, draw1;

      if( !Global.aim_spot.IsZero() )
        if( DrawManager::WorldToScreen( Global.aim_spot, draw0 ) && DrawManager::WorldToScreen( pLocal->GetAbsOrigin(), draw1 ) ) {
          DrawManager::DrawLine( draw0.x, draw0.y, draw1.x, draw1.y, Global.color_aim.get_color() );
          DrawManager::DrawRect( draw0.x - 5, draw0.y - 5, 10, 10, Global.color_aim.get_color() );
        }
    }

    if( Global.ESP_building_text.value || Global.ESP_item_text.value ) {
      for( int i = 1; i <= Int::EntityList->GetHighestEntityIndex(); i++ ) {
        CBaseEntity* pEnt = Int::EntityList->GetClientEntity( i );

        if( !pEnt ) {
          continue;
        }

        if( pEnt->IsDormant() ) {
          continue;
        }

        if( pEnt == pLocal ) {
          continue;
        }

        Vector center = pEnt->GetWorldSpaceCenter();
        Vector pos;
        std::string name = pEnt->GetClientClass()->name;

        if( DrawManager::WorldToScreen( center, pos ) && Util::Distance( pLocal->GetEyePosition(), center ) < 100 ) {
          if( Global.ESP_building_text.value ) {
            switch( (classId)pEnt->GetClassId() ) {
              case classId::CObjectDispenser:
                {
                  CObjectDispenser* pDispenser = (CObjectDispenser*)( pEnt );

                  if( !pDispenser->GetLevel() ) {
                    continue;
                  }

                  DrawManager::DrawString( pos.x, pos.y, Util::team_color( pLocal, pEnt ), L"Dispenser: " + std::to_wstring( pDispenser->GetLevel() ) );
                  pos.y += DrawManager::GetESPHeight();

                  if( pDispenser->IsSapped() ) {
                    DrawManager::DrawString( pos.x, pos.y, Colors::White, L"**SAPPED**", gFonts.anon );
                    pos.y += DrawManager::GetESPHeight();
                  }

                  DrawManager::DrawString( pos.x, pos.y, Colors::White, L"HP: " + std::to_wstring( pDispenser->GetHealth() ), gFonts.anon );
                  pos.y += DrawManager::GetESPHeight();
                  DrawManager::DrawString( pos.x, pos.y, Colors::White, L"Ammo: " + std::to_wstring( pDispenser->GetMetalReserve() ), gFonts.anon );
                  pos.y += DrawManager::GetESPHeight();
                  continue;
                }

              case classId::CObjectSentrygun:
                {
                  CObjectSentryGun* pSentryGun = (CObjectSentryGun*)( pEnt );

                  if( !pSentryGun->GetLevel() ) {
                    continue;
                  }

                  DrawManager::DrawString( pos.x, pos.y, Util::team_color( pLocal, pEnt ), L"Sentry: " + std::to_wstring( pSentryGun->GetLevel() ), gFonts.anon );
                  pos.y += DrawManager::GetESPHeight();

                  if( pSentryGun->IsSapped() ) {
                    DrawManager::DrawString( pos.x, pos.y, Colors::White, L"**SAPPED**", gFonts.anon );
                    pos.y += DrawManager::GetESPHeight();
                  }

                  DrawManager::DrawString( pos.x, pos.y, Colors::White, L"HP: " + std::to_wstring( pSentryGun->GetHealth() ), gFonts.anon );
                  pos.y += DrawManager::GetESPHeight();
                  DrawManager::DrawString( pos.x, pos.y, Colors::White, L"ammo: " + std::to_wstring( pSentryGun->GetAmmo() ), gFonts.anon );
                  pos.y += DrawManager::GetESPHeight();
                  DrawManager::DrawString( pos.x, pos.y, Colors::White, L"rockets: " + std::to_wstring( pSentryGun->GetRocket() ), gFonts.anon );
                  pos.y += DrawManager::GetESPHeight();
                  DrawManager::DrawString( pos.x, pos.y, Colors::White, pSentryGun->GetStateString(), gFonts.anon );
                  pos.y += DrawManager::GetESPHeight();

                  continue;
                }

              case classId::CObjectTeleporter:
                {
                  CObjectTeleporter* pTeleporter = (CObjectTeleporter*)( pEnt );

                  if( !pTeleporter->GetLevel() ) {
                    continue;
                  }

                  DrawManager::DrawString( pos.x, pos.y, Util::team_color( pLocal, pEnt ), L"Teleporter: " + std::to_wstring( pTeleporter->GetLevel() ), gFonts.anon );
                  pos.y += 11;

                  if( pTeleporter->IsSapped() ) {
                    DrawManager::DrawString( pos.x, pos.y, Colors::White, L"**SAPPED**" );
                    pos.y += 11;
                  }

                  DrawManager::DrawString( pos.x, pos.y, Colors::White, L"HP: " + std::to_wstring( pTeleporter->GetHealth() ), gFonts.anon );
                  pos.y += 11;
                  continue;
                }

              case classId::CCaptureFlag:
                {
                  if( Global.ESP_item_text.value ) {
                    DrawManager::DrawString( pos.x, pos.y, Util::team_color( pLocal, pEnt ), L"Intel", gFonts.anon, true );
                  }

                  continue;
                }
              case classId::CBaseAnimating:
              case classId::CTFAmmoPack:
                {
                  if( Global.ESP_item_text.value ) {
                    static std::unordered_map<std::string, std::wstring> cached_convert;

                    std::string model = Int::ModelInfo->GetModelName( pEnt->GetModel() );
                    int size = model.size();

                    auto cache = [&]( int start ) {
                      if( cached_convert.find( model ) == cached_convert.end() ) {
                        cached_convert[model] = ToWstring( model.c_str() ).substr( start, model.size() - start - 4 );
                      }
                    };

                    if( model.rfind( "models/items", 0 ) == 0 ) {
                      cache( 13 );
                      DrawManager::DrawString( pos.x, pos.y, Colors::White, cached_convert[model], gFonts.anon, true );
                      continue;
                    } else if( model.rfind( "models/props_halloween", 0 ) == 0 ) {
                      cache( 23 );
                      DrawManager::DrawString( pos.x, pos.y, Colors::White, cached_convert[model], gFonts.anon, true );
                      continue;
                    } else if( model.rfind( "models/props_medieval", 0 ) == 0 ) {
                      cache( 22 );
                      DrawManager::DrawString( pos.x, pos.y, Colors::White, cached_convert[model], gFonts.anon, true );
                      continue;
                    } else if( model.rfind( "models/pickups", 0 ) == 0 ) {
                      cache( 15 );
                      DrawManager::DrawString( pos.x, pos.y, Colors::White, cached_convert[model], gFonts.anon, true );
                      continue;
                    }
                  }
                }
            }
          }
        }
      }
    }

    if( Global.ESP_spectators.value ) {
      int SpectatorCount = 2;
      DrawManager::DrawString( 15, 15, Colors::White, L"Spectators:" );
      for( int i = 1; i <= Int::Engine->GetMaxClients(); i++ ) {

        CBaseEntity* pEntity = GetBaseEntity( i );

        if( !pEntity || pEntity->IsDormant() || pEntity->GetLifeState() == LIFE_ALIVE ) {
          continue;
        }

        player_info_t PlayerInfo;

        if( !Int::Engine->GetPlayerInfo( pEntity->GetIndex(), &PlayerInfo ) )
          continue;

        CBaseEntity* pObserverTarget = GetBaseEntityFromHandle( pEntity->ObserverTarget() );

        if( pObserverTarget == nullptr || pObserverTarget->IsDormant() )
          continue;

        if( pObserverTarget == pLocal && pObserverTarget->GetLifeState() == LIFE_ALIVE ) {
          int nObserverMode = pEntity->ObserverMode();

          if( nObserverMode == OBS_MODE_FIRSTPERSON || nObserverMode == OBS_MODE_THIRDPERSON ) {
            std::string name = PlayerInfo.name;

            if( nObserverMode == OBS_MODE_FIRSTPERSON ) {
              name += std::string( " - firstperson" );
            } else if( nObserverMode == OBS_MODE_THIRDPERSON ) {
              name += std::string( " - thirdperson" );
            }

            DrawManager::DrawString( 15, SpectatorCount++ * 15, Colors::White, name );
          }
        }
      }
    }
    if( Global.ESP_head_points.value ) {
      int my_index = me;
      int max = Int::Engine->GetMaxClients();

      for( int i = 1; i <= max; i++ ) {

        CBaseEntity* pEntity = GetBaseEntity( i );

        if( !pEntity || pEntity->GetLifeState() != LIFE_ALIVE ) {
          continue;
        }

        DWORD* model = pEntity->GetModel();

        if( !model ) {
          continue;
        }

        studiohdr_t* hdr = Int::ModelInfo->GetStudiomodel( model );

        if( !hdr ) {
          continue;
        }

        matrix3x4 matrix[128];

        if( !pEntity->SetupBones( matrix, 128, 0x100, Int::globals->curtime ) ) {
          continue;
        }

        mstudiohitboxset_t* set = hdr->GetHitboxSet( pEntity->GetHitboxSet() );

        if( !set ) {
          continue;
        }

        mstudiobbox_t* box = set->pHitbox( 0 );

        if( !box ) {
          continue;
        }

        Vector min = box->bbmin * 0.9f;
        Vector max = box->bbmax * 0.9f;

        Vector Points[]{
          Vector( min.x, min.y, min.z ),
          Vector( min.x, max.y, min.z ),
          Vector( min.x, min.y, max.z ),
          Vector( min.x, max.y, max.z ),
          Vector( max.x, min.y, min.z ),
          Vector( max.x, max.y, min.z ),
          Vector( max.x, min.y, max.z ),
          Vector( max.x, max.y, max.z ),
        };

        Vector Box;
        Vector temp;

        for( auto point : Points ) {
          Util::vector_transform( point, matrix[box->bone], Box );
          if( DrawManager::WorldToScreen( Box, temp ) ) {
            if( pLocal->CanSee( pEntity, Box ) )
              DrawManager::DrawRect( temp.x - 2, temp.y - 2, 4, 4, Colors::Orange );
          }
        }
      }
    }
  }
}