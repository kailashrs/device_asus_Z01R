/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "TouchscreenGestureService"

#include "TouchscreenGesture.h"
#include <android-base/logging.h>
#include <fstream>

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

const std::map<int32_t, TouchscreenGesture::GestureInfo> TouchscreenGesture::kGestureInfoMap = {
    {0, {251, "Two fingers down swipe", "/proc/touchscreen/double_swipe_enable"}},
    {1, {252, "Up arrow", "/proc/touchscreen/up_arrow_enable"}},
    {2, {254, "Right arrow", "/proc/touchscreen/right_arrow_enable"}},
    {3, {255, "Down arrow", "/proc/touchscreen/down_arrow_enable"}},
    {4, {253, "Left arrow", "/proc/touchscreen/left_arrow_enable"}},
    {5, {260, "One finger up swipe", "/proc/touchscreen/up_swipe_enable"}},
    {6, {263, "One finger right swipe", "/proc/touchscreen/right_swipe_enable"}},
    {7, {261, "One finger down swipe", "/proc/touchscreen/down_swipe_enable"}},
    {8, {262, "One finger left swipe", "/proc/touchscreen/left_swipe_enable"}},
    {9, {46, "Letter C", "/proc/touchscreen/letter_c_enable"}},
    {10, {50, "Letter M", "/proc/touchscreen/letter_m_enable"}},
    {11, {24, "Letter O", "/proc/touchscreen/letter_o_enable"}},
    {12, {31, "Letter S", "/proc/touchscreen/letter_s_enable"}},
    {13, {47, "Letter V","/proc/touchscreen/letter_v_enable"}},
    {14, {17, "Letter W", "/proc/touchscreen/letter_w_enable"}},
    {15, {44, "Letter Z", "/proc/touchscreen/letter_z_enable"}},
    {16, {67, "Single Tap", "/proc/touchscreen/single_tap_enable"}},
    {17, {143, "Double Tap", "/proc/touchscreen/double_tap_enable"}},
};

Return<void> TouchscreenGesture::getSupportedGestures(getSupportedGestures_cb resultCb) {
    std::vector<Gesture> gestures;

    for (const auto& entry : kGestureInfoMap) {
        if (access(entry.second.path, F_OK) != -1) {
            gestures.push_back({entry.first, entry.second.name, entry.second.keycode});
        }
    }
    resultCb(gestures);

    return Void();
}

Return<bool> TouchscreenGesture::setGestureEnabled(
    const ::vendor::lineage::touch::V1_0::Gesture& gesture, bool enabled) {
    const auto entry = kGestureInfoMap.find(gesture.id);
    if (entry == kGestureInfoMap.end()) {
        return false;
    }

    std::ofstream file(entry->second.path);
    file << (enabled ? "1" : "0");
    LOG(DEBUG) << "Wrote file " << entry->second.path << " fail " << file.fail();
    return !file.fail();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor