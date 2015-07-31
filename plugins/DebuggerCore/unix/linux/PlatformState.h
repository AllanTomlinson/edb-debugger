/*
Copyright (C) 2006 - 2014 Evan Teran
                          eteran@alum.rit.edu

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLATFORMSTATE_20110330_H_
#define PLATFORMSTATE_20110330_H_

#include "IState.h"
#include "Types.h"
#include <cstddef>
#include <sys/user.h>

namespace DebuggerCore {

static constexpr std::size_t DBG_REG_COUNT=8;
static constexpr std::size_t SEG_REG_COUNT=6;
static constexpr std::size_t FPU_REG_COUNT=8;
static constexpr std::size_t MMX_REG_COUNT=FPU_REG_COUNT;
#if defined EDB_X86
static constexpr std::size_t GPR_COUNT=8;
static constexpr std::size_t GPR_LOW_ADDRESSABLE_COUNT=4; // al,cl,dl,bl
#elif defined EDB_X86_64
static constexpr std::size_t GPR_COUNT=16;
static constexpr std::size_t GPR_LOW_ADDRESSABLE_COUNT=16; // all GPRs' low bytes are addressable
#endif
static constexpr std::size_t GPR_HIGH_ADDRESSABLE_COUNT=4; // ah,ch,dh,bh
static constexpr std::size_t XMM_REG_COUNT=GPR_COUNT;
static constexpr std::size_t YMM_REG_COUNT=GPR_COUNT;
static constexpr std::size_t ZMM_REG_COUNT=(IS_X86_32_BIT ? 8 : 32);

static constexpr bool dbgIndexValid(std::size_t n) { return n<DBG_REG_COUNT; }
static constexpr bool gprIndexValid(std::size_t n) { return n<GPR_COUNT; }
static constexpr bool fpuIndexValid(std::size_t n) { return n<FPU_REG_COUNT; }
static constexpr bool mmxIndexValid(std::size_t n) { return n<MMX_REG_COUNT; }
static constexpr bool xmmIndexValid(std::size_t n) { return n<XMM_REG_COUNT; }
static constexpr bool ymmIndexValid(std::size_t n) { return n<YMM_REG_COUNT; }
static constexpr bool zmmIndexValid(std::size_t n) { return n<ZMM_REG_COUNT; }

#ifdef EDB_X86
typedef struct user_regs_struct UserRegsStructX86;
typedef struct user_fpregs_struct UserFPRegsStructX86;
typedef struct user_fpxregs_struct UserFPXRegsStructX86;
// Dummies to avoid missing compile-time checks for conversion code.
// Actual layout is irrelevant since the code is not going to be executed
struct UserFPRegsStructX86_64 {
	uint16_t cwd;
	uint16_t swd;
	uint16_t ftw;
	uint16_t fop;
	uint64_t rip;
	uint64_t rdp;
	uint32_t mxcsr;
	uint32_t mxcr_mask;
	uint32_t st_space[32];
	uint32_t xmm_space[64];
	uint32_t padding[24];
};
struct UserRegsStructX86_64 {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t rbp;
	uint64_t rbx;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rax;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t orig_rax;
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t fs_base;
	uint64_t gs_base;
	uint64_t ds;
	uint64_t es;
	uint64_t fs;
	uint64_t gs;
};
#elif defined EDB_X86_64
typedef user_regs_struct UserRegsStructX86_64;
typedef user_fpregs_struct UserFPRegsStructX86_64;
// Dummies to avoid missing compile-time checks for conversion code
// Actual layout is irrelevant since the code is not going to be executed
struct UserRegsStructX86 {
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t esi;
	uint32_t edi;
	uint32_t ebp;
	uint32_t eax;
	uint32_t xds;
	uint32_t xes;
	uint32_t xfs;
	uint32_t xgs;
	uint32_t orig_eax;
	uint32_t eip;
	uint32_t xcs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t xss;
};
struct UserFPXRegsStructX86 {
	uint16_t cwd;
	uint16_t swd;
	uint16_t twd;
	uint16_t fop;
	uint32_t fip;
	uint32_t fcs;
	uint32_t foo;
	uint32_t fos;
	uint32_t mxcsr;
	uint32_t reserved;
	uint32_t st_space[32];   /* 8*16 bytes for each FP-reg = 128 bytes */
	uint32_t xmm_space[32];  /* 8*16 bytes for each XMM-reg = 128 bytes */
	uint32_t padding[56];
};
struct UserFPRegsStructX86 {
	uint32_t cwd;
	uint32_t swd;
	uint32_t twd;
	uint32_t fip;
	uint32_t fcs;
	uint32_t foo;
	uint32_t fos;
	uint32_t st_space [20];
};
#endif

class PlatformState : public IState {
	friend class DebuggerCore;

public:
	PlatformState();

public:
	virtual IState *clone() const;

public:
	virtual QString flags_to_string() const;
	virtual QString flags_to_string(edb::reg_t flags) const;
	virtual Register value(const QString &reg) const;
	virtual edb::address_t frame_pointer() const;
	virtual edb::address_t instruction_pointer() const;
	virtual edb::address_t stack_pointer() const;
	virtual edb::reg_t debug_register(int n) const;
	virtual edb::reg_t flags() const;
	virtual int fpu_stack_pointer() const;
	virtual edb::value80 fpu_register(int n) const;
	virtual bool fpu_register_is_empty(std::size_t n) const;
	virtual QString fpu_register_tag_string(std::size_t n) const;
	virtual edb::value16 fpu_control_word() const;
	virtual edb::value16 fpu_status_word() const;
	virtual edb::value16 fpu_tag_word() const;
	virtual void adjust_stack(int bytes);
	virtual void clear();
	virtual void set_debug_register(int n, edb::reg_t value);
	virtual void set_flags(edb::reg_t flags);
	virtual void set_instruction_pointer(edb::address_t value);
	virtual void set_register(const QString &name, edb::reg_t value);
	virtual edb::value64 mmx_register(int n) const;
	virtual edb::value128 xmm_register(int n) const;
	virtual Register gp_register(int n) const;

private:
	// The whole AVX* state. XMM and YMM registers are lower parts of ZMM ones.
	class AVX {
		std::array<edb::value512,ZMM_REG_COUNT> zmmStorage;
	public:
		edb::value32 mxcsr;
		edb::value32 mxcsrMask;
		bool xmmFilled=false;
		bool ymmFilled=false;
		bool zmmFilled=false;
		bool mxcsrMaskFilled=false;

		void clear();
		edb::value128 xmm(std::size_t index) const;
		void setXMM(std::size_t index,edb::value128);
		edb::value256 ymm(std::size_t index) const;
		void setYMM(std::size_t index,edb::value256);
		edb::value512 zmm(std::size_t index) const;
		void setZMM(std::size_t index,edb::value512);

		static constexpr const char* mxcsrName="mxcsr";
	} avx;

	// x87 state
	struct X87 {
		std::array<edb::value80,FPU_REG_COUNT> R; // Rx registers
		edb::address_t instPtrOffset;
		edb::address_t dataPtrOffset;
		edb::value16 instPtrSelector;
		edb::value16 dataPtrSelector;
		edb::value16 controlWord;
		edb::value16 statusWord;
		edb::value16 tagWord;
		edb::value16 opCode;
		bool filled=false;
		bool opCodeFilled=false;

		void clear();
		std::size_t stackPointer() const;
		// Convert from ST(n) index n to Rx index x
		std::size_t stIndexToRIndex(std::size_t index) const;
		// Restore the full FPU Tag Word from the ptrace-filtered version
		edb::value16 restoreTagWord(uint16_t twd) const;
		int tag(std::size_t n) const;
		enum Tag {
			TAG_VALID=0,
			TAG_ZERO=1,
			TAG_SPECIAL=2,
			TAG_EMPTY=3
		};
	private:
		int recreateTag(const edb::value80 value) const;
		int makeTag(std::size_t n, uint16_t twd) const;
	} x87;

	// i386-inherited (and expanded on x86_64) state
	struct X86 {
		std::array<edb::reg_t,GPR_COUNT> GPRegs;
		std::array<edb::reg_t,DBG_REG_COUNT> dbgRegs;
		edb::reg_t orig_ax;
		edb::reg_t flags; // whole flags register: EFLAGS/RFLAGS
		edb::address_t IP; // program counter: EIP/RIP
		std::array<edb::seg_reg_t,SEG_REG_COUNT> segRegs;
		edb::address_t fsBase;
		edb::address_t gsBase;
		bool segBasesFilled=false;
		bool filled=false;

		void clear();

		enum GPRIndex : std::size_t {
			EAX,RAX=EAX,
			ECX,RCX=ECX,
			EDX,RDX=EDX,
			EBX,RBX=EBX,
			ESP,RSP=ESP,
			EBP,RBP=EBP,
			ESI,RSI=ESI,
			EDI,RDI=EDI,
			R8,
			R9,
			R10,
			R11,
			R12,
			R13,
			R14,
			R15
		};
		enum SegRegIndex : std::size_t {
			ES,
			CS,
			SS,
			DS,
			FS,
			GS
		};
		static constexpr const char* IPName=(sizeof(edb::address_t)==8?"rip":"eip");
		static constexpr const char* flagsName=(sizeof(edb::reg_t)==8?"rflags":"eflags");
		static constexpr const char* fsBaseName="fs_base";
		static constexpr const char* gsBaseName="gs_base";
		// gcc 4.8 fails to understand inline initialization of std::array, so define these the old way
		static const std::array<const char*,GPR_COUNT> GPRegNames;
		static const std::array<const char*,GPR_COUNT> GPReg32Names;
		static const std::array<const char*,GPR_COUNT> GPReg16Names;
		static const std::array<const char*,GPR_LOW_ADDRESSABLE_COUNT> GPReg8LNames;
		static const std::array<const char*,GPR_HIGH_ADDRESSABLE_COUNT> GPReg8HNames;
		static const std::array<const char*,SEG_REG_COUNT> segRegNames;
	} x86;

	void fillFrom(const UserRegsStructX86& regs);
	void fillFrom(const UserFPRegsStructX86& regs);
	void fillFrom(const UserFPXRegsStructX86& regs);
	void fillFrom(const UserRegsStructX86_64& regs);
	void fillFrom(const UserFPRegsStructX86_64& regs);

	void fillStruct(UserRegsStructX86& regs) const;
	void fillStruct(UserRegsStructX86_64& regs) const;
};

}

#endif

