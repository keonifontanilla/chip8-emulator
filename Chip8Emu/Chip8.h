#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <fstream>
#include <chrono>
#include <random>

const unsigned int DISPLAY_WIDTH = 64;
const unsigned int DISPLAY_HEIGHT = 32;

class Chip8
{
public:
	uint8_t display[DISPLAY_WIDTH * DISPLAY_HEIGHT];
	uint8_t key[16];
	bool drawflag;

	Chip8();
	~Chip8();

	void EmulateCycle();
	void LoadRom(const char* filename);

private:
	uint8_t memory[4096];
	uint8_t V[16];
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint8_t sp;
	uint16_t I;
	uint16_t pc;
	uint16_t stack[16];
	uint16_t opcode;

	std::default_random_engine randGen;
	std::uniform_int_distribution<int> randByte;

	void Init();
	void Opcodes_All();
	void Opcodes_0();
	void Opcodes_8();
	void Opcodes_E();
	void Opcodes_F();

	// Opcodes
	void Opcode_Null();
	void Opcode_00E0();
	void Opcode_00EE();
	void Opcode_1nnn();
	void Opcode_2nnn();
	void Opcode_3xkk();
	void Opcode_4xkk();
	void Opcode_5xy0();
	void Opcode_6xkk();
	void Opcode_7xkk();
	void Opcode_8xy0();
	void Opcode_8xy1();
	void Opcode_8xy2();
	void Opcode_8xy3();
	void Opcode_8xy4();
	void Opcode_8xy5();
	void Opcode_8xy6();
	void Opcode_8xy7();
	void Opcode_8xyE();
	void Opcode_9xy0();
	void Opcode_Annn();
	void Opcode_Bnnn();
	void Opcode_Cxkk();
	void Opcode_Dxyn();
	void Opcode_Ex9E();
	void Opcode_ExA1();
	void Opcode_Fx07();
	void Opcode_Fx0A();
	void Opcode_Fx15();
	void Opcode_Fx18();
	void Opcode_Fx1E();
	void Opcode_Fx29();
	void Opcode_Fx33();
	void Opcode_Fx55();
	void Opcode_Fx65();

	using Chip8Instructions = void(Chip8::*)();
	Chip8Instructions opcodes_0xxx_Fxxx[0xF + 1];
	Chip8Instructions opcodes_0xxx[0xE + 1] = { &Chip8::Opcode_Null };
	Chip8Instructions opcodes_8xxx[0xE + 1] = { &Chip8::Opcode_Null };
	Chip8Instructions opcodes_Exxx[0xE + 1] = { &Chip8::Opcode_Null };
	Chip8Instructions opcodes_Fxxx[0x65 + 1] = { &Chip8::Opcode_Null };
};

#endif // !1

