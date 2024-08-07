/*
    WARNING: This file is NOT MIT licensed! It is adapted from Scrambled Bugs under non-transferable permission from KernalsEgg.
             See the license section of README.md for further explanation.
*/

#include "PowerAttackStaminaRequirement.h"
#include "SKSE/Trampoline.h"

#include "Addresses.h"

namespace Patch
{
    void PowerAttackStaminaRequirement::InstallPatch() {
        SKSE::Trampoline& trampoline = SKSE::GetTrampoline();

        if(settings.powerAttacksStaminaRequirementNPC)
        {
            REL::safe_write(RE::Address::PowerAttackStaminaChecks::ActorHasStamina.address(), REL::NOP2, 2);

            SKSE::AllocTrampoline(1 << 4);
            originalActorHasStamina_ = trampoline.write_call<5>(RE::Address::PowerAttackStaminaChecks::GetActorAttackStamina.address(), ActorHasStamina);

            logger::info("\"PowerAttackStaminaRequirementNPC\" patch installed!");
        }
        else
        {
            logger::info("\"PowerAttackStaminaRequirementNPC\" patch is NOT enabled... skipping.");
        }
    
        if(settings.powerAttacksStaminaRequirementPlayer)
        {
            REL::safe_write(RE::Address::PowerAttackStaminaChecks::PlayerHasStamina.address(), REL::NOP2, 2);
            
            SKSE::AllocTrampoline(1 << 4);
            originalPlayerHasStamina_ = trampoline.write_call<5>(RE::Address::PowerAttackStaminaChecks::GetPlayerAttackStamina.address(), PlayerHasStamina);

            logger::info("\"PowerAttackStaminaRequirementPlayer\" patch installed!");
        }
        else
        {
            logger::info("\"PowerAttackStaminaRequirementPlayer\" patch is NOT enabled... skipping.");
        }
    };
    
    float PowerAttackStaminaRequirement::ActorHasStamina(RE::ActorValueOwner* actorValueOwner, RE::BGSAttackData* attackData){
        float attackStamina = originalActorHasStamina_(actorValueOwner, attackData);

        float currentStamina = actorValueOwner->GetActorValue(RE::ActorValue::kStamina);

        if (attackStamina <= 0.0)
        {
            return attackStamina;
        }
        else if (currentStamina >= attackStamina)
        {
            return 0.0;
        }
        else
        {
            return attackStamina;
        }
    };
    
    float PowerAttackStaminaRequirement::PlayerHasStamina(RE::ActorValueOwner* actorValueOwner, RE::BGSAttackData* attackData){
        float attackStamina = originalPlayerHasStamina_(actorValueOwner, attackData);

        float currentStamina = actorValueOwner->GetActorValue(RE::ActorValue::kStamina);

        if (attackStamina <= 0.0)
        {
            return attackStamina;
        }
        else if (currentStamina >= attackStamina)
        {
            return 0.0;
        }
        else
        {
            return attackStamina;
        }
    };

    REL::Relocation<decltype(PowerAttackStaminaRequirement::ActorHasStamina)> PowerAttackStaminaRequirement::originalActorHasStamina_{};
    REL::Relocation<decltype(PowerAttackStaminaRequirement::ActorHasStamina)> PowerAttackStaminaRequirement::originalPlayerHasStamina_{};
} // namespace Patch