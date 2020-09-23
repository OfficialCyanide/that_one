#include "backtrack.h"
#include "../../tools/util/util.h"
std::deque<LagRecord> BacktrackData[64];
constexpr int BONE_USED_BY_ANYTHING = 0x7FF00;
namespace Backtrack {
  void collect_tick( CBaseEntity* pLocal ) {

    for( int i = 1; i <= Int::Engine->GetMaxClients(); i++ ) {
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

      if( pEntity->GetTeamNum() == pLocal->GetTeamNum() ) {
        continue;
      }

      Vector hitbox = pEntity->GetHitbox( pLocal, Global.hitbox != -1 ? Global.hitbox : 0, true );

      BacktrackData[i].emplace_front( LagRecord{ false, pEntity->flSimulationTime(), Util::EstimateAbsVelocity( pEntity ), hitbox, pEntity->GetEyeAngles(), pEntity->GetWorldSpaceCenter() } );

      BacktrackData[i].front().valid = pEntity->SetupBones( BacktrackData[i].front().boneMatrix, 128, BONE_USED_BY_ANYTHING, Int::globals->curtime );

      if( BacktrackData[i].size() > 70 ) {
        BacktrackData[i].pop_back();
      }
    }
  }
  float lerp_time() {
    static ConVar* c_updaterate = Int::cvar->FindVar( "cl_updaterate" );
    static ConVar* c_minupdate = Int::cvar->FindVar( "sv_minupdaterate" );
    static ConVar* c_maxupdate = Int::cvar->FindVar( "sv_maxupdaterate" );
    static ConVar* c_lerp = Int::cvar->FindVar( "cl_interp" );
    static ConVar* c_cmin = Int::cvar->FindVar( "sv_client_min_interp_ratio" );
    static ConVar* c_cmax = Int::cvar->FindVar( "sv_client_max_interp_ratio" );
    static ConVar* c_ratio = Int::cvar->FindVar( "cl_interp_ratio" );
    float lerp = c_lerp->GetFloat();
    float maxupdate = c_maxupdate->GetFloat();
    int updaterate = c_updaterate->GetInt();
    float ratio = c_ratio->GetFloat();
    int sv_maxupdaterate = c_maxupdate->GetInt();
    int sv_minupdaterate = c_minupdate->GetInt();
    float cmin = c_cmin->GetFloat();
    float cmax = c_cmax->GetFloat();

    if( sv_maxupdaterate && sv_minupdaterate ) {
      updaterate = maxupdate;
    }

    if( ratio == 0 ) {
      ratio = 1.0f;
    }

    if( cmin && cmax && cmin != 1 ) {
      ratio = clamp( ratio, cmin, cmax );
    }

    return fmax( lerp, ratio / updaterate );
  }
  CachedINetChannel INetChannel_cache;
  void cache_INetChannel( INetChannel* ch ) {
    INetChannel_cache = CachedINetChannel( ch );
  }
  bool is_tick_valid( float simtime ) {
    float correct = 0;
    correct += INetChannel_cache.flow_incoming;
    correct += INetChannel_cache.flow_outgoing;
    correct += INetChannel_cache.lerptime;

    if( Global.latency.value ) {
      correct += ( Global.latency_amount.value + Global.ping_diff.value ) / 1000.0f;
    }

    correct = clamp( correct, 0.0f, 1.0f );
    float deltaTime = correct - ( Int::globals->curtime - simtime );
    return fabs( deltaTime ) <= 0.2f;
  }
}