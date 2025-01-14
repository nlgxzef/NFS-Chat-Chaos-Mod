/*
// clang-format off
//
//    NFS Chaos Mod (NFS-Chat-Chaos-Mod)
//    Copyright (C) 2022 Berkay Yigit <berkaytgy@gmail.com>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program. If not, see <https://www.gnu.org/licenses/>.
//
// clang-format on
*/

#pragma once
#include "pch.h"
#include "Helpers/OpenSpeedEx.hpp"
#include "Extensions/Game/IFakeGameEffect.hpp"

namespace Extensions::Game::MW05::Effects {
  class FakeTeleportToRandomRacer : public IFakeGameEffect {
    bool                   mSaved;
    OpenMW::UMath::Vector3 mSavedPosition;
    OpenMW::UMath::Vector3 mSavedDirection;
    OpenMW::UMath::Vector3 mSavedAngularVelocity;
    OpenMW::UMath::Vector3 mSavedLinearVelocity;

   protected:
    virtual bool _specialCooldownConditionSatisfied() const noexcept override { return OpenMW::PVehicleEx::GetRacerCars().size() > 0; }

    virtual bool _onRevealed() noexcept override {
      auto* pvehicle = OpenMW::PVehicleEx::GetPlayerInstance();
      if (!pvehicle) return false;

      if (!pvehicle->SetVehicleOnGround(mSavedPosition, mSavedDirection)) return false;
      pvehicle->GetRigidBody()->SetAngularVelocity(mSavedAngularVelocity);
      pvehicle->GetRigidBody()->SetLinearVelocity(mSavedLinearVelocity);
      mSaved = false;
      return true;
    }

    virtual bool _activate() noexcept override {
      auto* player_vehicle = OpenMW::PVehicleEx::GetPlayerInstance();
      if (!player_vehicle) return false;

      const auto& racers = OpenMW::PVehicleEx::GetRacerCars();
      if (racers.size() == 0) return false;

      auto* racer = racers[Random::Get().Generate(0, racers.size() - 1)];
      if (!racer || racer->IsPlayer() || racer->IsOwnedByPlayer()) return false;
      if (!racer->mRenderable || !racer->mRenderable->IsRenderable()) return false;
      if (racer->GetSpeed() < 1.0f) return false;

      // Save values
      if (!mSaved) {
        auto* rb       = player_vehicle->GetRigidBody();
        mSavedPosition = rb->GetPosition();
        rb->GetForwardVector(mSavedDirection);
        mSavedAngularVelocity = rb->GetAngularVelocity();
        mSavedLinearVelocity  = rb->GetLinearVelocity();
        mSaved                = true;
      }
      // Minimal Speed
      racer->SetSpeed(1.0f);
      player_vehicle->SetSpeed(1.0f);
      // Same direction (if possible)
      {
        auto* player_rb = player_vehicle->GetRigidBody() | OpenMW::RigidBodyEx::AsRigidBody;
        auto* racer_rb  = racer->GetRigidBody() | OpenMW::RigidBodyEx::AsRigidBody;
        if (player_rb && racer_rb) player_rb->SetRotation(racer_rb->GetRotation());
      }
      // Same position
      player_vehicle->GetRigidBody()->SetPosition(racer->GetRigidBody()->GetPosition());
      racer->GetRigidBody()->SetLinearVelocity(OpenMW::UMath::Vector3(0.0f, 0.0f, 10.0f));
      return true;
    }

   public:
    explicit FakeTeleportToRandomRacer() : IFakeGameEffect(118) {}
  };
}  // namespace Extensions::Game::MW05::Effects