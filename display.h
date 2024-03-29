// (C) Copyright 2020 Xo Wang <xo@geekshavefeelings.com>
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "esphome.h"

#include <array>
#include <cstdio>
#include <ctime>

// This file is included only by the main translation unit
namespace {

using namespace esphome::font;
using namespace esphome::display;
using namespace esphome::sensor;

struct Point {
  int x;
  int y;
};

struct Extents {
  Point p;
  int width;
  int height;
};

Extents GetTextExtents(Display& it,
                       Point start,
                       Font* font,
                       const std::string& text,
                       TextAlign align) {
  Extents extents;
  it.get_text_bounds(start.x, start.y, text.c_str(), font, align, &extents.p.x,
                     &extents.p.y, &extents.width, &extents.height);
  return extents;
}

[[gnu::format(printf, 1, 2)]] std::string StringFormat(const char* fmt, ...) {
  va_list args_measure;
  va_start(args_measure, fmt);
  va_list args_write;
  va_copy(args_write, args_measure);
  std::vector<char> output(std::vsnprintf(nullptr, 0, fmt, args_measure) + 1);
  va_end(args_measure);
  std::vsnprintf(output.data(), output.size(), fmt, args_write);
  va_end(args_write);
  return std::string(output.cbegin(), std::prev(output.cend()));
}

// Returns a point that moves down by the height of the text printed. Note that
// this only makes sense for top-aligned text.
Point DrawText(Display& it,
               Point start,
               Font* font,
               const std::string& text,
               TextAlign align,
               Color color = COLOR_ON) {
  it.print(start.x, start.y, font, color, align, text.c_str());
  const auto extents = GetTextExtents(it, start, font, text, align);
  return Point{start.x, extents.p.y + extents.height};
}

enum class FontSize {
  kTiny,
  kSmall,
  kNormal,
  kLarge,
  kHuge,
};

Font* GetFont(FontSize size) {
  static Font* const kFonts[] = {&id(font_sans_12), &id(font_sans_17),
                                 &id(font_sans_24), &id(font_sans_34),
                                 &id(font_sans_48)};
  return kFonts[static_cast<size_t>(size)];
}

struct Card {
  std::string label;
  Sensor* sensor;
  Color color;
};

struct Page {
  std::array<Card, 2> cards;
};

void DrawPage(Display& it, size_t page_index) {
  // Must be function statically initialized because id() can't be called at
  // namespace init time.
  // Colors from https://colorbrewer2.org/#type=qualitative&scheme=Set3&n=6
  static std::array kPages{Page{{Card{.label = u8"CO₂",
                                      .sensor = &id(iaq_scd30_co2),
                                      .color = Color(0x8dd3c7)},
                                 Card{.label = "Total VOCs",
                                      .sensor = &id(iaq_svm30_tvoc),
                                      .color = Color(0xffffb3)}}},
                           Page{{Card{.label = "PM 2.5",
                                      .sensor = &id(iaq_pm_2_5),
                                      .color = Color(0xbebada)},
                                 Card{.label = "PM 10",
                                      .sensor = &id(iaq_pm_10_0),
                                      .color = Color(0xfb8072)}}},
                           Page{{Card{.label = "Temperature",
                                      .sensor = &id(iaq_svm30_temperature),
                                      .color = Color(0x80b1d3)},
                                 Card{.label = "Humidity",
                                      .sensor = &id(iaq_svm30_humidity),
                                      .color = Color(0xfdb462)}}}};

  // Draw sensor reading cards from top to bottom
  Point cursor{(it.get_width() - 1) / 2, 0};
  for (auto& [label, sensor, color] : kPages.at(page_index).cards) {
    // Draw sensor label
    // TODO: fade_to_black doesn't work on mainline ESPHome yet
    // See https://github.com/esphome/esphome/pull/1300
    const auto color_shaded = color.fade_to_black(180);
    cursor = DrawText(it, cursor, GetFont(FontSize::kNormal), label,
                      TextAlign::TOP_CENTER, color_shaded);

    // Format and measure the sensor state value in huge text
    const auto value_str =
        StringFormat("%.*f", sensor->get_accuracy_decimals(), sensor->state);
    const auto value_font = GetFont(FontSize::kHuge);

    // Distance from top of value text to its baseline
    const int baseline_height =
        cursor.y - GetTextExtents(it, cursor, value_font, value_str,
                                  TextAlign::BASELINE_LEFT)
                       .p.y;
    auto [_, card_bottom] = DrawText(it, {0, cursor.y}, value_font, value_str,
                                     TextAlign::TOP_LEFT, color);

    // Draw units right-aligned at the same baseline as the state value
    DrawText(it, {it.get_width() - 1, cursor.y + baseline_height},
             GetFont(FontSize::kNormal), sensor->get_unit_of_measurement(),
             TextAlign::BASELINE_RIGHT, color_shaded);

    // Draw dividing space with a thin line
    const int kCardSpacing = 14;
    const Color kCardDividerColor(0x202020);
    it.horizontal_line(
        /*x=*/kCardSpacing / 2, /*y=*/card_bottom + kCardSpacing / 2,
        /*width=*/it.get_width() - kCardSpacing, /*color=*/kCardDividerColor);
    cursor = {cursor.x, card_bottom + kCardSpacing};
  }

  // Draw date and time at bottom
  Point bottom_center{cursor.x, it.get_height() - 1};
  cursor = DrawText(it, bottom_center, GetFont(FontSize::kNormal),
                    id(time_esp).now().strftime("%b %d %H:%M"),
                    TextAlign::BOTTOM_CENTER);
}

}  // namespace
