#include "modes/Fish.h"
#include <vector>

#define BLUE 0x0000FF
#define ORANGE 0xFF9B00

uint16_t small_fish[] = {
    0b10110,
    0b11111,
    0b10110};

uint16_t medium_fish[] = {
    0b000100,
    0b101110,
    0b111111,
    0b101110,
    0b000100};

uint16_t big_fish[] = {
    //0b000001000],
    0b100011100,
    0b110111110,
    0b111111111,
    0b110111110,
    0b100011100 //,
    //0b000001000]
};

uint16_t angelfish[] = {
    0b0011000,
    0b1001100,
    0b1101110,
    0b1111111,
    0b1101110,
    0b1001100,
    0b0011000,
};

#define SIZE_X(model) (model == small_fish ? 5 : (model == medium_fish ? 6 : (model == angelfish ? 7 : 9)))
#define SIZE_Y(model) (model == small_fish ? 3 : (model == medium_fish ? 5 : (model == angelfish ? 7 : 5)))

struct Fish
{
    uint8_t px, py;
    int8_t direction;
    uint16_t *model;

    Fish() : px(0), py(esp_random() % LED_HEIGHT), direction(0)
    {
        const auto r = esp_random() % (4 + 4 + 2 + 1);
        this->model = r < 4 ? small_fish : (r < 8 ? medium_fish : (r < 9 ? big_fish : angelfish));
    }

    void move(const Fish *other)
    {
        uint16_t r = esp_random() % 350;

        if (!other || this->px < (other->px - SIZE_X(other->model)) - 2)
        {
            if (r & 1)
                this->px++;
        }
        // random with weights (same direction: 48, no move 348, top: 1, down: 1)
        int8_t d = r < 48 ? this->direction : (r < 348 ? 0 : (r == 448 ? -1 : 1));
        if (0 < (this->py + d) && (this->py + d) < LED_HEIGHT)
        {
            this->py += d;
            this->direction = d;
        }
    }
};

struct _FishImpl
{
    std::vector<Fish> fish;
    uint16_t time = 0;
};

FishTank::FishTank() : impl(new _FishImpl())
{
    this->mode = MODE_FISHTANK;
}

FishTank::~FishTank()
{
    delete this->impl;
}

void FishTank::run(uint8_t delta)
{
    // check if we can spawn new fish
    if (impl->fish.size() == 0 || impl->fish[0].px > SIZE_X(impl->fish[0].model) + 1)
        if (esp_random() % 6 == 1)
            this->impl->fish.insert(this->impl->fish.begin(), Fish());

    // Move the fish, only if time is ok
    this->impl->time += delta;
    if (this->impl->time > (-190 * this->speed / 255 + 200))
    {
        this->impl->time -= (-190 * this->speed / 255 + 200);

        for (uint8_t idx = this->impl->fish.size() - 1; idx >= 0; --idx)
        {
            this->impl->fish[idx].move(idx + 1 < this->impl->fish.size() ? &this->impl->fish[idx + 1] : nullptr);
            // Check if we can remove some fish
            if (this->impl->fish[idx].px > LED_WIDTH)
                this->impl->fish.erase(this->impl->fish.begin() + idx);
        }
    }

    // Set background color
    for (uint8_t y = 0; y < LED_HEIGHT; ++y)
    {
        const CRGB color = (((LED_HEIGHT - y) * (200 / LED_HEIGHT)) << 8) | BLUE;
        for (uint8_t x = 0; x < LED_WIDTH; ++x)
            leds[XY(x, y)] = color;
    }
    // Render fish
    for (const Fish &fish : this->impl->fish)
    {
        for (uint8_t y = 0; y < LED_HEIGHT; ++y)
        {
            if (fish.py + SIZE_Y(fish.model) < y)
                break;
            if (fish.py > y)
                continue;
            for (uint8_t x = max(static_cast<uint8_t>(0), fish.px); x < min(fish.px + SIZE_X(fish.model), LED_WIDTH); ++x)
            {
                if (fish.model[y - fish.py] & (1 << (SIZE_X(fish.model) - 1 - x + fish.px)))
                    leds[XY(x, y)] = ORANGE;
            }
        }
    }
}
