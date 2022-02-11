#include "Chip8.h"
#include <iostream>

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONT_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;

uint8_t chip8Fontset[FONT_SIZE] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8::Init()
{
	pc = START_ADDRESS;
	opcode = 0;
	I = 0;
	sp = 0;

	// clear display
	for (int i = 0; i < 2048; ++i)
	{
		display[i] = 0;
	}

	// clear stack
	for (int i = 0; i < 16; ++i)
	{
		stack[i] = 0;
	}

	//clear registers
	for (int i = 0; i < 16; ++i)
	{
		V[i] = 0;
	}

	//clear memory
	for (int i = 0; i < 4096; ++i)
	{
		memory[i] = 0;
	}

	//clear key
	for (int i = 0; i < 16; ++i)
	{
		key[i] = 0;
	}

	// load fontset
	for (unsigned int i = 0; i < FONT_SIZE; ++i)
	{
		memory[FONTSET_START_ADDRESS + i] = chip8Fontset[i];
	}

	delayTimer = 0;
	soundTimer = 0;

	drawflag = true;

	Opcodes_All();
}

void Chip8::Opcodes_All()
{
	opcodes_0xxx_Fxxx[0x0] = &Chip8::Opcodes_0;
	opcodes_0xxx_Fxxx[0x1] = &Chip8::Opcode_1nnn;
	opcodes_0xxx_Fxxx[0x2] = &Chip8::Opcode_2nnn;
	opcodes_0xxx_Fxxx[0x3] = &Chip8::Opcode_3xkk;
	opcodes_0xxx_Fxxx[0x4] = &Chip8::Opcode_4xkk;
	opcodes_0xxx_Fxxx[0x5] = &Chip8::Opcode_5xy0;
	opcodes_0xxx_Fxxx[0x6] = &Chip8::Opcode_6xkk;
	opcodes_0xxx_Fxxx[0x7] = &Chip8::Opcode_7xkk;
	opcodes_0xxx_Fxxx[0x8] = &Chip8::Opcodes_8;
	opcodes_0xxx_Fxxx[0x9] = &Chip8::Opcode_9xy0;
	opcodes_0xxx_Fxxx[0xA] = &Chip8::Opcode_Annn;
	opcodes_0xxx_Fxxx[0xB] = &Chip8::Opcode_Bnnn;
	opcodes_0xxx_Fxxx[0xC] = &Chip8::Opcode_Cxkk;
	opcodes_0xxx_Fxxx[0xD] = &Chip8::Opcode_Dxyn;
	opcodes_0xxx_Fxxx[0xE] = &Chip8::Opcodes_E;
	opcodes_0xxx_Fxxx[0xF] = &Chip8::Opcodes_F;

	opcodes_0xxx[0x0] = &Chip8::Opcode_00E0;
	opcodes_0xxx[0xE] = &Chip8::Opcode_00EE;

	opcodes_8xxx[0x0] = &Chip8::Opcode_8xy0;
	opcodes_8xxx[0x1] = &Chip8::Opcode_8xy1;
	opcodes_8xxx[0x2] = &Chip8::Opcode_8xy2;
	opcodes_8xxx[0x3] = &Chip8::Opcode_8xy3;
	opcodes_8xxx[0x4] = &Chip8::Opcode_8xy4;
	opcodes_8xxx[0x5] = &Chip8::Opcode_8xy5;
	opcodes_8xxx[0x6] = &Chip8::Opcode_8xy6;
	opcodes_8xxx[0x7] = &Chip8::Opcode_8xy7;
	opcodes_8xxx[0xE] = &Chip8::Opcode_8xyE;

	opcodes_Exxx[0x1] = &Chip8::Opcode_ExA1;
	opcodes_Exxx[0xE] = &Chip8::Opcode_Ex9E;

	opcodes_Fxxx[0x7] = &Chip8::Opcode_Fx07;
	opcodes_Fxxx[0xA] = &Chip8::Opcode_Fx0A;
	opcodes_Fxxx[0x15] = &Chip8::Opcode_Fx15;
	opcodes_Fxxx[0x18] = &Chip8::Opcode_Fx18;
	opcodes_Fxxx[0x1E] = &Chip8::Opcode_Fx1E;
	opcodes_Fxxx[0x29] = &Chip8::Opcode_Fx29;
	opcodes_Fxxx[0x33] = &Chip8::Opcode_Fx33;
	opcodes_Fxxx[0x55] = &Chip8::Opcode_Fx55;
	opcodes_Fxxx[0x65] = &Chip8::Opcode_Fx65;
}

void Chip8::Opcodes_0()
{
	(this->*opcodes_0xxx[opcode & 0x000Fu])();
}

void Chip8::Opcodes_8()
{
	(this->*opcodes_8xxx[opcode & 0x000Fu])();
}

void Chip8::Opcodes_E()
{
	(this->*opcodes_Exxx[opcode & 0x000Fu])();
}

void Chip8::Opcodes_F()
{
	(this->*opcodes_Fxxx[opcode & 0x00FFu])();
}

void Chip8::Opcode_Null() {}

void Chip8::Opcode_00E0()
{
	memset(display, 0, sizeof(display));
}

void Chip8::Opcode_00EE()
{
	--sp;
	pc = stack[sp];
}

void Chip8::Opcode_1nnn()
{
	uint16_t address = opcode & 0x0FFFu;
	pc = address;
}

void Chip8::Opcode_2nnn()
{
	uint16_t address = opcode & 0x0FFFu;
	stack[sp] = pc;
	++sp;
	pc = address;
}

void Chip8::Opcode_3xkk()
{
	uint8_t Vx = V[(opcode & 0x0F00u) >> 8u];
	uint8_t byte = opcode & 0x00FFu;

	if (Vx == byte)
		pc += 2;
}

void Chip8::Opcode_4xkk()
{
	uint8_t Vx = V[(opcode & 0x0F00u) >> 8u];
	uint8_t byte = opcode & 0x00FFu;

	if (Vx != byte)
		pc += 2;
}

void Chip8::Opcode_5xy0()
{
	uint8_t Vx = V[(opcode & 0x0F00u) >> 8u];
	uint8_t Vy = V[(opcode & 0x00F0u) >> 4u];

	if (Vx == Vy)
		pc += 2;
}

void Chip8::Opcode_6xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	V[Vx] = byte;
}

void Chip8::Opcode_7xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	V[Vx] += byte;
}

void Chip8::Opcode_8xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] = V[Vy];
}

void Chip8::Opcode_8xy1()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	
	V[Vx] |= V[Vy];
}

void Chip8::Opcode_8xy2()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] &= V[Vy];
}

void Chip8::Opcode_8xy3()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	V[Vx] ^= V[Vy];
}

void Chip8::Opcode_8xy4()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint16_t sum = V[Vx] + V[Vy];

	if (sum > 255u)
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[Vx] = sum & 0x00FFu;
}

void Chip8::Opcode_8xy5()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vx] > V[Vy])
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[Vx] -= V[Vy];
}

void Chip8::Opcode_8xy6()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if ((V[Vx] & 0x1u) == 0x1u)
		V[0xF] = 1;
	else
		V[0x0F] = 0;

	V[Vx] >>= 1;
}

void Chip8::Opcode_8xy7()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vy] > V[Vx])
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[Vx] = V[Vy] - V[Vx];
}

void Chip8::Opcode_8xyE()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if (((V[Vx] & 0x80u) >> 7u) == 0x1u)
		V[0xF] = 1;
	else
		V[0x0F] = 0;

	V[Vx] <<= 1;
}

void Chip8::Opcode_9xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vx] != V[Vy])
		pc += 2;
}

void Chip8::Opcode_Annn()
{
	uint16_t address = opcode & 0x0FFFu;
	
	I = address;
}

void Chip8::Opcode_Bnnn()
{
	uint16_t address = opcode & 0x0FFFu;

	pc = address + V[0];
}

void Chip8::Opcode_Cxkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	V[Vx] = randByte(randGen) & byte;
}

void Chip8::Opcode_Dxyn()
{
	uint8_t x = V[(opcode & 0x0F00u) >> 8u] % DISPLAY_WIDTH;
	uint8_t y = V[(opcode & 0x00F0u) >> 4u] % DISPLAY_HEIGHT;
	uint8_t height = opcode & 0x000Fu;

	V[0xF] = 0;
	for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t pixel = memory[I + row];
		for (unsigned int col = 0; col < 8; ++col)
		{
			uint32_t offset = ((y + row) * DISPLAY_WIDTH + x + col) & 0x07FFu;
			
			if ((pixel & (0x80u >> col)) != 0)
			{
				if (display[offset] == 1)
					V[0xF] = 1;
				display[offset] ^= 1;
			}
		}
	}

	drawflag = true;
}

void Chip8::Opcode_Ex9E()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if (key[V[Vx]])
		pc += 2;
}

void Chip8::Opcode_ExA1()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if (!key[V[Vx]])
		pc += 2;
}

void Chip8::Opcode_Fx07()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	V[Vx] = delayTimer;
}

void Chip8::Opcode_Fx0A()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	bool keyPressed = false;

	for (int i = 0; i < 16; ++i)
	{
		if (key[i] != 0)
		{
			V[Vx] = i;
			keyPressed = true;
		}
	}

	if (!keyPressed) pc -= 2;
}

void Chip8::Opcode_Fx15()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	delayTimer = V[Vx];
}

void Chip8::Opcode_Fx18()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	soundTimer = V[Vx];
}

void Chip8::Opcode_Fx1E()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	I += V[Vx];
}

void Chip8::Opcode_Fx29()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digitStartIndex = FONTSET_START_ADDRESS + (5 * V[Vx]);

	I = digitStartIndex;
}

void Chip8::Opcode_Fx33()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8;

	memory[I] = V[Vx] / 100;
	memory[I + 1] = (V[Vx] / 10) % 10;
	memory[I + 2] = V[Vx] % 10;
}

void Chip8::Opcode_Fx55()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (unsigned int i = 0; i <= Vx; ++i)
	{
		memory[I + i] = V[i];
	}
}

void Chip8::Opcode_Fx65()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (unsigned int i = 0; i <= Vx; ++i)
	{
		V[i] = memory[I + i];
	}
}

Chip8::Chip8()
	: randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
	Init();
	randByte = std::uniform_int_distribution<int>(0, 255U);
}

Chip8::~Chip8()
{
}

void Chip8::EmulateCycle()
{
	opcode = memory[pc] << 8u | memory[pc + 1];
	pc += 2;

	(this->*opcodes_0xxx_Fxxx[(opcode & 0xF000u) >> 12u])();
}

void Chip8::TimerCycle()
{
	if (delayTimer > 0)
		--delayTimer;

	if (soundTimer > 0)
		--soundTimer;
}

bool Chip8::LoadRom(const char* filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (!file) return false;

	if (file.is_open())
	{
		std::streampos size = file.tellg();
		std::vector<uint8_t> buffer(size);

		file.seekg(0, std::ios::beg);
		file.read(reinterpret_cast<char*>(buffer.data()), size);
		file.close();

		for (int i = 0; i < size; ++i)
		{
			memory[START_ADDRESS + i] = buffer[i];
		}
	}
	else
		return false;

	return true;
}
