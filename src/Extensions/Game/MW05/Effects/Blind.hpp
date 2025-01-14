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
  class Blind : public IGameEffect {
   protected:
    virtual void _draw(const ImGuiViewport* viewport) noexcept override {
      ImGui::WithColor _c1(ImGuiCol_WindowBg, IM_COL32_BLACK);

      ImGui::SetNextWindowBgAlpha(1.0f);
      ImGui::SetNextWindowPos(viewport->Pos, ImGuiCond_Always);
      ImGui::SetNextWindowSize(viewport->Size * 2.0f, ImGuiCond_Always);
      ImGui::Begin("##blind_effect_window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBringToFrontOnFocus);
      ImGui::End();
    };

   public:
    explicit Blind() : IGameEffect(103) {}
  };
}  // namespace Extensions::Game::MW05::Effects
