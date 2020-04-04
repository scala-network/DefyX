/*
Copyright (c) 2018-2019, tevador <tevador@gmail.com>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holder nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <cstdint>
#include "common.hpp"
#include "program.hpp"

/* Global namespace for C binding */
class defyx_vm {
public:
	virtual ~defyx_vm() = 0;
	virtual void allocate() = 0;
	virtual void getFinalResult(void* out, size_t outSize) = 0;
	virtual void hashAndFill(void* out, size_t outSize, uint64_t *fill_state) = 0;
	virtual void setDataset(defyx_dataset* dataset) { }
	virtual void setCache(defyx_cache* cache) { }
	virtual void initScratchpad(void* seed) = 0;
	virtual void run(void* seed) = 0;
	void resetRoundingMode();
	defyx::RegisterFile *getRegisterFile() {
		return &reg;
	}
	const void* getScratchpad() {
		return scratchpad;
	}
	const defyx::Program& getProgram()
	{
		return program;
	}
protected:
	void initialize();
	alignas(64) defyx::Program program;
	alignas(64) defyx::RegisterFile reg;
	alignas(16) defyx::ProgramConfiguration config;
	defyx::MemoryRegisters mem;
	uint8_t* scratchpad = nullptr;
	union {
		defyx_cache* cachePtr = nullptr;
		defyx_dataset* datasetPtr;
	};
	uint64_t datasetOffset;
public:
	std::string cacheKey;
	alignas(16) uint64_t tempHash[8]; //8 64-bit values used to store intermediate data
};

namespace defyx {

	template<class Allocator, bool softAes>
	class VmBase : public defyx_vm {
	public:
		~VmBase() override;
		void allocate() override;
		void initScratchpad(void* seed) override;
		void getFinalResult(void* out, size_t outSize) override;
		void hashAndFill(void* out, size_t outSize, uint64_t *fill_state) override;
	protected:
		void generateProgram(void* seed);
	};

}
