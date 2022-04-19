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
#include "Extensions/Game/IGameEffect.hpp"
#include "Helpers/FMODWrapper.hpp"

namespace Extensions::Game {
  class IFakeGameEffect : public IGameEffect {
   protected:
    // Fake Name
    const std::string mFakeName;

    // Fake description
    const std::string mFakeDescription;

    // Should reveal
    mutable bool mShouldReveal;

    // Has revealed
    mutable bool mHasRevealed;

    // Reveal delay
    const std::chrono::milliseconds mRevealDelay;

    virtual bool _onRevealed() noexcept { return true; }

    bool ShouldReveal() const {
      if (this->GetStatus() != Status::Active) {
        mShouldReveal = false;
        mHasRevealed  = false;
        return true;  // intentional to make fake name visible on voting
      }
      if (mShouldReveal) return true;

      mShouldReveal = (std::chrono::steady_clock::now() - mTimeActivated) > mRevealDelay;
      return mShouldReveal;
    }

    /// Fake name & description

    virtual const std::string& GetName() const override {
      if (ShouldReveal()) return IGameEffect::GetName();
      return mFakeName;
    }
    virtual const std::string& GetDescription() const override {
      if (ShouldReveal()) return IGameEffect::GetDescription();
      return mFakeDescription;
    }

    /// Active tick

    virtual void ActiveTick() override {
      IGameEffect::ActiveTick();
      if (GetStatus() == Status::Active) {
        if (mShouldReveal && !mHasRevealed) {
          if (_onRevealed()) {
            FMODWrapper::Get().PlaySoundFX(FMODWrapper::SoundFX::Sike);
            mHasRevealed = true;
          }
        }
      }
    }

    /// ctor

    explicit IFakeGameEffect(std::uint32_t effectIndex) :
        IGameEffect(effectIndex),
        mFakeName(std::move(Config::Get()["Effects"][effectIndex]["FakeName"].Get<std::string>())),
        mFakeDescription(std::move(Config::Get()["Effects"][effectIndex]["FakeDescription"].Get<std::string>())),
        mShouldReveal(false),
        mHasRevealed(false),
        mRevealDelay(std::move(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<double>(Config::Get()["Effects"][effectIndex]["RevealDelay"].GetDouble())))) {}
  };
}  // namespace Extensions::Game
