#pragma once
#include "../menu/base/controls.h"
#include "headers/vector.h"
#include "../tools/dumps/dumps.h"
#include <utility>
bool checkExists( std::string file );
void SaveToJson();
void LoadFromJson();

struct CGlobalVariables {
  int backtrack_tick = 0;
  int backtrack_arr = 0;
  int aim_index = -1;
  int aim_mode = -1;
  Vector aim_spot;

  int hitbox = -1;
  int last_cmd_number = 0;

  Checkbox Aimbot_enable{ "Enable aimbot", true };
  Slider   Aimbot_fov{ "Fov", 5, 1, 180, 1 };
  Checkbox Aimbot_proj_lazy{ "Lazy proj fov", true };
  Listbox  Aimbot_hitbox{ "Hitbox", { "nearest mouse", "first visible", "head", "body" }, 1, 130 };
  Checkbox Aimbot_silent{ "Silent", true };
  Checkbox Aimbot_range{ "Range check", true };
  Slider   Aimbot_ranges{ "Shotgun range", 26, 1, 100, 1 };
  Checkbox Aimbot_melee{ "Melee aim", true };
  Checkbox Aimbot_multipoint{ "Multipoint", true };
  Checkbox Backtrack{ "Backtrack", true };
  KeyBind  Aimbot_auto_aim{ "Aim key", VK_SHIFT, e_kbmode::always };
  KeyBind  Aimbot_auto_shoot{ "Shoot key", VK_SHIFT, e_kbmode::always };

  Groupbox AIMBOT{ "--Aimbot--", {
    &Aimbot_enable,
    &Aimbot_fov,
    &Aimbot_proj_lazy,
    &Aimbot_hitbox,
    &Aimbot_silent,
    &Aimbot_range,
    &Aimbot_ranges,
    &Aimbot_melee,
    &Aimbot_multipoint,
    &Backtrack,
    &Aimbot_auto_aim,
    &Aimbot_auto_shoot
    }, 170
  };

  Checkbox Airblast_enable{ "Enable airblast", true };
  Checkbox Airblast_silent{ "Silent aim", true };

  Groupbox PYRO{ "--Pyro--", {
    &Airblast_enable, &Airblast_silent
    }
  };

  Checkbox demo_sticky{ "Auto sticky", true };

  Groupbox DEMOMAN{ "--Demoman--", {
    &demo_sticky
    }
  };

  Checkbox sniper_zoomed{ "Zoomed only", true };
  Checkbox sniper_delay{ "Wait for headshot", true };
  Checkbox sniper_nozoom{ "No zoom" };
  Checkbox sniper_noscope{ "No scope", true };

  Groupbox SNIPER{ "--Sniper--", {
    &sniper_zoomed,
    &sniper_delay,
    &sniper_nozoom,
    &sniper_noscope
    }
  };

  Checkbox Ignore_A_cloak{ "ignore cloak", true };
  Checkbox Ignore_A_disguise{ "ignore disguise", true };
  Checkbox Ignore_A_taunt{ "ignore taunt" };

  Groupbox IGNORE_A{ "--Ignore--", {
    &Ignore_A_cloak,
    &Ignore_A_disguise,
    &Ignore_A_taunt
    }
  };

  Tab Aimbot{ "Aimbot", {
    &AIMBOT,
    &PYRO,
    &DEMOMAN,
    &SNIPER,
    &IGNORE_A
    }
  };

  Checkbox ESP_enable{ "Enable ESP", true };
  Checkbox ESP_enemy{ "Enemy only" };

  Groupbox ESP_BASE{ "--ESP--", {
    &ESP_enable,
    &ESP_enemy
    }
  };

  Checkbox ESP_building_text{ "Building text", true };
  Checkbox ESP_item_text{ "Item names", true };

  Groupbox ESP_TEXT{ "--Text esp--", {
    &ESP_building_text,
    &ESP_item_text
    }
  };

  Listbox  ESP_cham_mat{ "Cham material", { "shaded", "glow"}, 0 };
  Listbox  ESP_object{ "Object esp",{"none","cham","glow"}, 1 };
  Listbox  ESP_building{ "Building esp",{"none","cham","glow"}, 1 };
  Listbox  ESP_player{ "Player esp",{"none","cham","glow"}, 1 };
  Checkbox ESP_backtrack{ "Backtrack cham", true };
  Checkbox ESP_proj_cham{ "Projectile cham", true };
  Checkbox ESP_hat{ "No player items" };
  Listbox  ESP_hand{ "Hands", { "normal", "none", "transparent", "mat" }, 0 };

  Groupbox ESP_CHAM{ "--Chams--", {
    &ESP_cham_mat,
    &ESP_object,
    &ESP_building,
    &ESP_player,
    &ESP_backtrack,
    &ESP_proj_cham,
    &ESP_hat,
    &ESP_hand,
    }
  };

  Listbox  ESP_fov{ "Fov circle", {"none", "normal", "dotted" }, 2 };
  Listbox  ESP_target{ "Target highlight", { "Off", "line", "color", "both" }, 1 };
  Checkbox ESP_around_fov{ "Out of fov esp", true };
  Checkbox ESP_spectators{ "Spectator list", true };
  Checkbox ESP_head_points{ "Head point esp", true };

  Groupbox ESP_MISC{ "--Misc--", {
    &ESP_fov,
    &ESP_target,
    &ESP_around_fov,
    &ESP_spectators,
    &ESP_head_points
    }
  };

  Checkbox Ignore_E_cloak{ "ignore cloak", true };
  Checkbox Ignore_E_disguise{ "ignore disguise" };

  Groupbox IGNORE_E{ "--Ignore--", {
    &Ignore_E_cloak,
    &Ignore_E_disguise
    }
  };

  Tab ESP{ "ESP", {
    &ESP_BASE,
    &ESP_TEXT,
    &ESP_CHAM,
    &ESP_MISC,
    &IGNORE_E
    }
  };

  ColorPicker color_objects{ "Objects", Colors::Green };
  ColorPicker color_building{ "Buildings", Colors::White };

  Groupbox color_world_obj{ "--World obj cham--", {
    &color_objects,
    &color_building
    }
  };

  Listbox     color_type{ "Cham color type", { "red/blue", "enemy/ally" } };
  ColorPicker color_red{ "Red", Colors::Red };
  ColorPicker color_blue{ "Blue", Colors::Blue };
  ColorPicker color_enemy{ "Enemy", Colors::Orange };
  ColorPicker color_ally{ "Ally", Colors::Indigo };

  Groupbox color_cham{ "--Player cham--", {
    &color_type,
    &color_red,
    &color_blue,
    &color_enemy,
    &color_ally
    }
  };

  ColorPicker color_cham_history{ "Backtrack history", Colors::White };
  ColorPicker color_cham_tick{ "Backtrack tick", Colors::Green };

  Groupbox color_backtrack{ "--Backtrack cham--", {
    &color_cham_history,
    &color_cham_tick
    }
  };

  ColorPicker color_aim{ "Target color", Colors::Violet };
  ColorPicker color_fov{ "Fov circle", Colors::Yellow, true };

  Groupbox color_misc{ "--Misc color--", {
    &color_aim,
    &color_fov
    }
  };

  Checkbox    world_enabled{ "World/Sky color" };
  ColorPicker world_clr{ "World color", Color( 200 ) };
  ColorPicker sky_clr{ "Sky color", Color( 200 ) };

  Groupbox color_world{ "--World color--", {
    &world_enabled,
    &world_clr,
    &sky_clr
    }
  };

  Tab Colors{ "Color", {
    &color_world_obj,
    &color_cham,
    &color_backtrack,
    &color_misc,
    &color_world
    }
  };

  Checkbox Bunnyhop{ "Bunny hop", true };
  Checkbox Autostrafe{ "Auto strafe", true };

  Groupbox MOVEMENT{ "--Movement--", {
    &Bunnyhop,
    &Autostrafe
    }
  };

  Checkbox NoRecoil{ "No visual recoil", true };
  Checkbox NoPush{ "No push", true };

  Groupbox MISC{ "--Misc--", {
    &NoRecoil,
    &NoPush
    }
  };

  Checkbox engine{ "Engine prediction", true };
  Checkbox engine_edgejump{ "Edge jump", true };
  Groupbox ENGINE{ "--Engine pred--", {
    &engine,
    &engine_edgejump
    }
  };

  Checkbox latency{ "Latency" };
  Slider latency_amount{ "Added latency", 0, 0, 800, 10, 120 };
  Slider ping_diff{ "Ping difference", 0, -100, 100, 10, 140 };

  Groupbox LATENCY{ "--Latency--", {
    &latency,
    &latency_amount,
    &ping_diff,
    }
  };

  Checkbox explanation1{ "200 ping for 400ms backtrack", comment };
  Checkbox explanation2{ "or 800 ping for the 1 sec backtrack", comment };

  Checkbox fake{ "Fake Angle" };
  Slider fake_amount{ "fake lag",0,0,14,1 };

  Groupbox FAKE{ "--Fakes--", {
    &fake,
    &fake_amount,
    }
  };

  Tab Misc{ "Misc", {
    &MOVEMENT,
    &MISC,
    &ENGINE,
    &LATENCY,
    &explanation1,
    &explanation2,
    &FAKE
    }
  };

  Checkbox    loadbyclass{ "Load based on class" };
  Functionbox savejson{ "Save to json", SaveToJson };
  Functionbox loadjson{ "Load from json", LoadFromJson };

  Groupbox CONFIG{ "--Config--", {
    &loadbyclass,
    &savejson,
    &loadjson
    }, 200
  };

  Tab Config{ "Config", {
    &CONFIG
    }
  };

  Checkbox Sv_cheat{ "Sv_cheat 1" };
  KeyBind Thirdperson{ "Thirdperson", 0x54, e_kbmode::disabled };
  Checkbox Thirdperson_scoped{ "Draw scoped player" };
  Functionbox Dump_classId{ "Dump classId", Dumps::dumpClassIds };
  Functionbox Dump_netvar{ "Dump netvar", Dumps::dumpNetvars };

  Tab Testing{ "Testing", {
    &Sv_cheat,
    &Thirdperson,
    &Thirdperson_scoped,
    &Dump_classId,
    &Dump_netvar,
    }
  };

  TabGroup tf2{ {
    &Aimbot,
    &ESP,
    &Misc,
    &Colors,
    &Config,
    &Testing
    }
  };
};