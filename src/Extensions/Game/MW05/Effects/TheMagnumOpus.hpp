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

namespace Extensions::Game::MW05::Effects {
  class TheMagnumOpus : public IGameEffect {
   protected:
    virtual bool _specialCooldownConditionSatisfied() const noexcept override {
      return OpenMW::GameStatusEx::HasFinishedPrologue() && !OpenMW::GameStatusEx::IsInPursuit();
    }

    virtual bool _activate() noexcept override {
      auto* pvehicle = OpenMW::PVehicleEx::GetPlayerInstance();
      if (!pvehicle) return false;

      auto* db = OpenMW::cFrontEndDatabase::Get();
      if (!db) return false;

      auto* db_car_record = db->GetUserProfile()->mPlayersCarStable.GetCarRecordByHandle(db->GetCareerSettings().GetCurrentCar());
      if (!db_car_record) return false;

      auto instance = OpenMW::Attrib::Gen::pvehicle::TryGetInstance(db_car_record->mVehicleKey);
      if (!instance.mCollection) return false;

      // Install random visual tuning
      OpenMW::RideInfo ride_info(db_car_record->GetType());
      ride_info.SetStockParts();
      ride_info.SetRandomPaint();
      ride_info.SetRandomParts();
      ride_info.SetUpgradePart(OpenMW::CarSlotId::Spoiler, OpenMW::eCareerUpgradeLevels::Level1);
      OpenMW::FECustomizationRecord customizations;
      customizations.WriteRideIntoRecord(&ride_info);
      // Install max perf parts
      customizations.mInstalledPhysics = OpenMW::PhysicsEx::GetMaxLevelPackage(instance);
      customizations.mTunings[0]       = OpenMW::Physics::Tunings(2.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f);
      customizations.mActiveTuning     = OpenMW::eCustomTuningType::Setting1;
      // Change car
      if (!OpenMW::PVehicleEx::ChangePlayerVehicle(db_car_record->mVehicleKey, &customizations)) return false;
      // Save to DB
      OpenMW::FEDatabaseEx::ChangeCarData(db_car_record, db_car_record->mVehicleKey, &customizations);
      return true;
    }

   public:
    explicit TheMagnumOpus() : IGameEffect(17) {}
  };
}  // namespace Extensions::Game::MW05::Effects
