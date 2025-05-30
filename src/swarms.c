#include "global.h"
#include "pokemon.h"
#include "battle.h"
#include "swarms.h"
#include "string_util.h"
#include "level_caps.h"
#include "mail.h"
#include "pokemon_storage_system.h"
#include "event_data.h"
#include "random.h"
#include "main.h"
#include "egg_hatch.h"
#include "text.h"
#include "menu.h"
#include "international_string_util.h"
#include "script.h"
#include "strings.h"
#include "task.h"
#include "window.h"
#include "party_menu.h"
#include "list_menu.h"
#include "overworld.h"
#include "pokedex.h"
#include "item.h"
#include "constants/form_change_types.h"
#include "constants/items.h"
#include "constants/hold_effects.h"
#include "constants/moves.h"
#include "constants/region_map_sections.h"

u8 swarmsActivated = 0;

bool8 AreSwarmsDone(void)
{
    return swarmsActivated == NUM_SWARMS_FLAGS;
}

void UpdateSwarmsActivated(void)
{
    swarmsActivated = 0;

    // Swarms are activated after beating the game, so no need to check otherwise
    if (!FlagGet(FLAG_SYS_GAME_CLEAR))
        return;

    for (u8 i = 0; i < NUM_SWARMS_FLAGS; i++)
    {
        if (FlagGet(FLAG_SWARMS_START + i) == TRUE)
        {
            swarmsActivated++;
        }
    }
}

u16 GetSwarmSpeciesFlag(u16 species)
{
    switch (species)
    {
    case SPECIES_CATERPIE:
    case SPECIES_METAPOD:
    case SPECIES_BUTTERFREE:
        return FLAG_CATERPIE_SPECIES_SWARMING;
    case SPECIES_WEEDLE:
    case SPECIES_KAKUNA:
    case SPECIES_BEEDRILL:
        return FLAG_WEEDLE_SPECIES_SWARMING;
    case SPECIES_PIDGEY:
    case SPECIES_PIDGEOTTO:
    case SPECIES_PIDGEOT:
        return FLAG_PIDGEY_SPECIES_SWARMING;
    case SPECIES_RATTATA:
    case SPECIES_RATICATE:
        return FLAG_RATTATA_SPECIES_SWARMING;
    case SPECIES_SPEAROW:
    case SPECIES_FEAROW:
        return FLAG_SPEAROW_SPECIES_SWARMING;
    case SPECIES_EKANS:
    case SPECIES_ARBOK:
        return FLAG_EKANS_SPECIES_SWARMING;
    case SPECIES_NIDORAN_F:
    case SPECIES_NIDORINA:
    case SPECIES_NIDOQUEEN:
    case SPECIES_NIDORAN_M:
    case SPECIES_NIDORINO:
    case SPECIES_NIDOKING:
        return FLAG_NIDORAN_SPECIES_SWARMING;
    case SPECIES_CLEFAIRY:
    case SPECIES_CLEFABLE:
        return FLAG_CLEFAIRY_SPECIES_SWARMING;
    case SPECIES_MEOWTH:
    case SPECIES_PERSIAN:
        return FLAG_MEOWTH_SPECIES_SWARMING;
    case SPECIES_MANKEY:
    case SPECIES_PRIMEAPE:
        return FLAG_MANKEY_SPECIES_SWARMING;
    case SPECIES_GROWLITHE:
    case SPECIES_ARCANINE:
        return FLAG_GROWLITHE_SPECIES_SWARMING;
    case SPECIES_POLIWAG:
    case SPECIES_POLIWHIRL:
    case SPECIES_POLIWRATH:
    case SPECIES_POLITOED:
        return FLAG_POLIWAG_SPECIES_SWARMING;
    case SPECIES_BELLSPROUT:
    case SPECIES_WEEPINBELL:
    case SPECIES_VICTREEBEL:
        return FLAG_BELLSPROUT_SPECIES_SWARMING;
    case SPECIES_PONYTA:
    case SPECIES_RAPIDASH:
        return FLAG_PONYTA_SPECIES_SWARMING;
    case SPECIES_SLOWPOKE:
    case SPECIES_SLOWBRO:
    case SPECIES_SLOWKING:
        return FLAG_SLOWPOKE_SPECIES_SWARMING;
    case SPECIES_SEEL:
    case SPECIES_DEWGONG:
        return FLAG_SEEL_SPECIES_SWARMING;
    case SPECIES_SHELLDER:
    case SPECIES_CLOYSTER:
        return FLAG_SHELLDER_SPECIES_SWARMING;
    case SPECIES_GASTLY:
    case SPECIES_HAUNTER:
    case SPECIES_GENGAR:
        return FLAG_GASTLY_SPECIES_SWARMING;
    case SPECIES_ONIX:
    case SPECIES_STEELIX:
        return FLAG_ONIX_SPECIES_SWARMING;
    case SPECIES_DROWZEE:
    case SPECIES_HYPNO:
        return FLAG_DROWZEE_SPECIES_SWARMING;
    case SPECIES_KRABBY:
    case SPECIES_KINGLER:
        return FLAG_KRABBY_SPECIES_SWARMING;
    case SPECIES_EXEGGCUTE:
    case SPECIES_EXEGGUTOR:
        return FLAG_EXEGGCUTE_SPECIES_SWARMING;
    case SPECIES_CUBONE:
    case SPECIES_MAROWAK:
        return FLAG_CUBONE_SPECIES_SWARMING;
    case SPECIES_LICKITUNG:
        return FLAG_LICKITUNG_SPECIES_SWARMING;
    case SPECIES_CHANSEY:
    case SPECIES_BLISSEY:
        return FLAG_CHANSEY_SPECIES_SWARMING;
    case SPECIES_TANGELA:
        return FLAG_TANGELA_SPECIES_SWARMING;
    case SPECIES_KANGASKHAN:
        return FLAG_KANGASKHAN_SPECIES_SWARMING;
    case SPECIES_SCYTHER:
    case SPECIES_SCIZOR:
        return FLAG_SCYTHER_SPECIES_SWARMING;
    case SPECIES_SMOOCHUM:
    case SPECIES_JYNX:
        return FLAG_JYNX_SPECIES_SWARMING;
    case SPECIES_ELEKID:
    case SPECIES_ELECTABUZZ:
        return FLAG_ELECTABUZZ_SPECIES_SWARMING;
    case SPECIES_MAGBY:
    case SPECIES_MAGMAR:
        return FLAG_MAGMAR_SPECIES_SWARMING;
    case SPECIES_LAPRAS:
        return FLAG_LAPRAS_SPECIES_SWARMING;
    case SPECIES_DRATINI:
    case SPECIES_DRAGONAIR:
    case SPECIES_DRAGONITE:
        return FLAG_DRATINI_SPECIES_SWARMING;
    case SPECIES_SENTRET:
    case SPECIES_FURRET:
        return FLAG_SENTRET_SPECIES_SWARMING;
    case SPECIES_HOOTHOOT:
    case SPECIES_NOCTOWL:
        return FLAG_HOOTHOOT_SPECIES_SWARMING;
    case SPECIES_LEDYBA:
    case SPECIES_LEDIAN:
        return FLAG_LEDYBA_SPECIES_SWARMING;
    case SPECIES_SPINARAK:
    case SPECIES_ARIADOS:
        return FLAG_SPINARAK_SPECIES_SWARMING;
    case SPECIES_MAREEP:
    case SPECIES_FLAAFFY:
    case SPECIES_AMPHAROS:
        return FLAG_MAREEP_SPECIES_SWARMING;
    case SPECIES_HOPPIP:
    case SPECIES_SKIPLOOM:
    case SPECIES_JUMPLUFF:
        return FLAG_HOPPIP_SPECIES_SWARMING;
    case SPECIES_MURKROW:
        return FLAG_MURKROW_SPECIES_SWARMING;
    case SPECIES_MISDREAVUS:
        return FLAG_MISDREAVUS_SPECIES_SWARMING;
    case SPECIES_DUNSPARCE:
        return FLAG_DUNSPARCE_SPECIES_SWARMING;
    case SPECIES_GLIGAR:
        return FLAG_GLIGAR_SPECIES_SWARMING;
    case SPECIES_SNEASEL:
        return FLAG_SNEASEL_SPECIES_SWARMING;
    case SPECIES_REMORAID:
    case SPECIES_OCTILLERY:
        return FLAG_REMORAID_SPECIES_SWARMING;
    case SPECIES_HOUNDOUR:
    case SPECIES_HOUNDOOM:
        return FLAG_HOUNDOUR_SPECIES_SWARMING;
    case SPECIES_MANTINE:
        return FLAG_MANTINE_SPECIES_SWARMING;
    default:
        return -1;
    }
}

u16 GetSpeciesFromSwarmFlag(u16 swarmFlag)
{
    switch (swarmFlag)
    {
    case FLAG_CATERPIE_SPECIES_SWARMING:
        return SPECIES_CATERPIE;
    case FLAG_WEEDLE_SPECIES_SWARMING:
        return SPECIES_WEEDLE;
    case FLAG_PIDGEY_SPECIES_SWARMING:
        return SPECIES_PIDGEY;
    case FLAG_RATTATA_SPECIES_SWARMING:
        return SPECIES_RATTATA;
    case FLAG_SPEAROW_SPECIES_SWARMING:
        return SPECIES_SPEAROW;
    case FLAG_EKANS_SPECIES_SWARMING:
        return SPECIES_EKANS;
    case FLAG_NIDORAN_SPECIES_SWARMING:
        return SPECIES_NIDORAN_F;
        // NIDORAN_M species is handled by the code that calls this function
        // return SPECIES_NIDORAN_M;
    case FLAG_CLEFAIRY_SPECIES_SWARMING:
        return SPECIES_CLEFAIRY;
    case FLAG_MEOWTH_SPECIES_SWARMING:
        return SPECIES_MEOWTH;
    case FLAG_MANKEY_SPECIES_SWARMING:
        return SPECIES_MANKEY;
    case FLAG_GROWLITHE_SPECIES_SWARMING:
        return SPECIES_GROWLITHE;
    case FLAG_POLIWAG_SPECIES_SWARMING:
        return SPECIES_POLIWAG;
    case FLAG_BELLSPROUT_SPECIES_SWARMING:
        return SPECIES_BELLSPROUT;
    case FLAG_PONYTA_SPECIES_SWARMING:
        return SPECIES_PONYTA;
    case FLAG_SLOWPOKE_SPECIES_SWARMING:
        return SPECIES_SLOWPOKE;
    case FLAG_SEEL_SPECIES_SWARMING:
        return SPECIES_SEEL;
    case FLAG_SHELLDER_SPECIES_SWARMING:
        return SPECIES_SHELLDER;
    case FLAG_GASTLY_SPECIES_SWARMING:
        return SPECIES_GASTLY;
    case FLAG_ONIX_SPECIES_SWARMING:
        return SPECIES_ONIX;
    case FLAG_DROWZEE_SPECIES_SWARMING:
        return SPECIES_DROWZEE;
    case FLAG_KRABBY_SPECIES_SWARMING:
        return SPECIES_KRABBY;
    case FLAG_EXEGGCUTE_SPECIES_SWARMING:
        return SPECIES_EXEGGCUTE;
    case FLAG_CUBONE_SPECIES_SWARMING:
        return SPECIES_CUBONE;
    case FLAG_LICKITUNG_SPECIES_SWARMING:
        return SPECIES_LICKITUNG;
    case FLAG_CHANSEY_SPECIES_SWARMING:
        return SPECIES_CHANSEY;
    case FLAG_TANGELA_SPECIES_SWARMING:
        return SPECIES_TANGELA;
    case FLAG_KANGASKHAN_SPECIES_SWARMING:
        return SPECIES_KANGASKHAN;
    case FLAG_SCYTHER_SPECIES_SWARMING:
        return SPECIES_SCYTHER;
    case FLAG_JYNX_SPECIES_SWARMING:
        return SPECIES_JYNX;
    case FLAG_ELECTABUZZ_SPECIES_SWARMING:
        return SPECIES_ELECTABUZZ;
    case FLAG_MAGMAR_SPECIES_SWARMING:
        return SPECIES_MAGMAR;
    case FLAG_LAPRAS_SPECIES_SWARMING:
        return SPECIES_LAPRAS;
    case FLAG_DRATINI_SPECIES_SWARMING:
        return SPECIES_DRATINI;
    case FLAG_SENTRET_SPECIES_SWARMING:
        return SPECIES_SENTRET;
    case FLAG_HOOTHOOT_SPECIES_SWARMING:
        return SPECIES_HOOTHOOT;
    case FLAG_LEDYBA_SPECIES_SWARMING:
        return SPECIES_LEDYBA;
    case FLAG_SPINARAK_SPECIES_SWARMING:
        return SPECIES_SPINARAK;
    case FLAG_MAREEP_SPECIES_SWARMING:
        return SPECIES_MAREEP;
    case FLAG_HOPPIP_SPECIES_SWARMING:
        return SPECIES_HOPPIP;
    case FLAG_MURKROW_SPECIES_SWARMING:
        return SPECIES_MURKROW;
    case SPECIES_MISDREAVUS:
    case FLAG_MISDREAVUS_SPECIES_SWARMING:
        return SPECIES_MISDREAVUS;
    case FLAG_DUNSPARCE_SPECIES_SWARMING:
        return SPECIES_DUNSPARCE;
    case FLAG_GLIGAR_SPECIES_SWARMING:
        return SPECIES_GLIGAR;
    case FLAG_SNEASEL_SPECIES_SWARMING:
        return SPECIES_SNEASEL;
    case FLAG_REMORAID_SPECIES_SWARMING:
        return SPECIES_REMORAID;
    case FLAG_HOUNDOUR_SPECIES_SWARMING:
        return SPECIES_HOUNDOUR;
    case FLAG_MANTINE_SPECIES_SWARMING:
        return SPECIES_MANTINE;
    default:
        return -1;
    }
}

void ActivateNewSwarm()
{
    u16 inactiveSwarms[NUM_SWARMS_FLAGS], inactive = 0;
    
    for (u8 i = 0; i < NUM_SWARMS_FLAGS; i++)
    {
        if (FlagGet(FLAG_SWARMS_START + i) == FALSE)
        {
            inactiveSwarms[inactive++] = FLAG_SWARMS_START + i;
        }
    }
    ActivateSwarmSpecies(inactiveSwarms[Random() % inactive]);
}

void ActivateSwarmSpecies(u16 swarmFlag)
{
    FlagSet(swarmFlag);
    swarmsActivated++;
}

bool8 HasSpeciesSwarmHappened(u16 species)
{
    u16 flag = GetSwarmSpeciesFlag(species);
    return flag == -1 ? FALSE : FlagGet(flag);
}

u16 GetNextUnshowSpecies()
{
    for (u8 i = 0; i < NUM_SWARMS_FLAGS; i++)
    {
        u16 species = GetSpeciesFromSwarmFlag(FLAG_SWARMS_START + i);
        if (!HasSpeciesSwarmHappened(species))
            continue;
        
        if (!GetSetPokedexFlag(species, FLAG_GET_SEEN))
            return species;
        else if (species == SPECIES_NIDORAN_F && !GetSetPokedexFlag(SPECIES_NIDORAN_M, FLAG_GET_SEEN))
            return SPECIES_NIDORAN_M;
    }
    return SPECIES_NONE;    
}