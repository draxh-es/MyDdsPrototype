#pragma once
#include <cstdint>

constexpr uint32_t MAGIC = 0x44445331;//dds 1 ascıı kodu

struct MesajBasligi {
	uint32_t magic;
	uint32_t topic_id;
	uint32_t sequence;
	uint32_t payload;


};



