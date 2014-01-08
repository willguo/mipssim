#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "mips.h" // for execute_syscall()
#include "types.h"

void execute_instruction(Instruction instruction,Processor* processor,Byte *memory) {
    
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    switch(instruction.opcode) {
        case 0x0: // opcode == 0x0(SPECIAL)
            switch(instruction.rtype.funct) {
                case 0xc: // funct == 0xc (SYSCALL)
                    execute_syscall(processor);
                    processor->PC += 4;
                    break;
                case 0x24: // funct == 0x24 (AND)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] & processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
    	        case 0x00: // funct == 0x00 (SLL)
                        processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] << instruction.rtype.shamt;
                        processor->PC += 4;
                        break;
    		    case 0x02: // funct == 0x02 (SRL)
                        processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] >> instruction.rtype.shamt;
                        processor->PC += 4;
                        break;
    	        case 0x03: // funct == 0x03 (SRA)
                        processor->R[instruction.rtype.rd] = (int32_t)processor->R[instruction.rtype.rt] >> instruction.rtype.shamt;
                        processor->PC += 4;
                        break;
    	        case 0x08: // funct == 0x08 (JR)
                        processor->PC = processor->R[instruction.rtype.rs];
                        break;
          	    case 0x09: // funct == 0x09 (JALR)
                    {
                        int tmp;
                        tmp = processor->PC + 4;
                        processor->PC = processor->R[instruction.rtype.rs];
    		            processor->R[instruction.rtype.rd] = tmp;
                        break;
                    }
    	        case 0x10: // funct == 0x10 (MFHI)
                        processor->R[instruction.rtype.rd] = processor->RHI;
                        processor->PC += 4;
                        break;
    	        case 0x12: // funct == 0x12 (MFLO)
                        processor->R[instruction.rtype.rd] = processor->RLO;
                        processor->PC += 4;
                        break;
    	        case 0x18: // funct == 0x18 (MULT)
                    {
                        int tmp = (int32_t)(processor->R[instruction.rtype.rs]) * (int32_t)(processor->R[instruction.rtype.rt]);
                        processor->RHI = tmp & 0xffffffff00000000;
                        processor->RLO = tmp & 0x00000000ffffffff;
                        processor->PC += 4;
                        break;
                    }
           	    case 0x19: // funct == 0x19 (MULTU)
                    {   
                        int tmp = (processor->R[instruction.rtype.rs]) * (processor->R[instruction.rtype.rt]);
                        processor->RHI = tmp & 0xffffffff00000000;
                        processor->RLO = tmp & 0x00000000ffffffff;
                        processor->PC += 4;
                        break;
                    }
    	        case 0x21: // funct == 0x21 (ADDU)
                        processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] + processor->R[instruction.rtype.rt];
                        processor->PC += 4;
                        break;
    	        case 0x23: // funct == 0x23 (SUBU)
                        processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] - processor->R[instruction.rtype.rt];
                        processor->PC += 4;
                        break;
    	        case 0x25: // funct == 0x25 (OR)
                        processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt];
                        processor->PC += 4;
                        break;
    	        case 0x26: // funct == 0x26 (XOR)
                        processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] ^ processor->R[instruction.rtype.rt];
                        processor->PC += 4;
                        break;
    	        case 0x27: // funct == 0x27 (NOR)
                        processor->R[instruction.rtype.rd] = ~(processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt]);
                        processor->PC += 4;
                        break;
    	        case 0x2a: // funct == 0x2a (SLT)
                        processor->R[instruction.rtype.rd] = (int32_t)processor->R[instruction.rtype.rs] < (int32_t)processor->R[instruction.rtype.rt];
                        processor->PC += 4;
                        break;
    	        case 0x2b: // funct == 0x2b (SLTU)
                        processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] < processor->R[instruction.rtype.rt];
                        processor->PC += 4;
                        break;
                default: // undefined funct
                        fprintf(stderr,"%s: pc=%08x,illegal function=%08x\n",__FUNCTION__,processor->PC,instruction.bits);
                        exit(-1);
                        break;
                }
                break;
                
        case 0x2: // opcode == 0x2 (J)
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
	    case 0x3: // opcode == 0x3 (JAL)
	        processor->R[31] = processor->PC + 4;
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
        case 0xd: // opcode == 0xd (ORI)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] | instruction.itype.imm;
            processor->PC += 4;
            break;    
	    case 0x04: // opcode == 0x04 (BEQ)
	        if(processor->R[instruction.itype.rs] == processor->R[instruction.itype.rt]) {
		    processor->PC = processor->PC + 4 + ((int32_t)(int16_t)(instruction.itype.imm))*4;
	        } else {
	            processor->PC += 4;
            }
            break;
	    case 0x05: // opcode == 0x05 (BNE)
            if(processor->R[instruction.itype.rs] != processor->R[instruction.itype.rt]) {
		    processor->PC = processor->PC + 4 + ((int32_t)(int16_t)(instruction.itype.imm))*4;
	        } else {
	            processor->PC += 4;
            }
            break;
        case 0x09: // opcode == 0x09 (ADDIU)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] + (int32_t)(int16_t)(instruction.itype.imm);
            processor->PC += 4;
            break;
        case 0x0a: // opcode == 0x0a (SLTI) 
            processor->R[instruction.itype.rt] = (int32_t)processor->R[instruction.itype.rs] < (int32_t)(int16_t)(instruction.itype.imm);
            processor->PC += 4;
            break;
        case 0x0b: // opcode == 0x0b (SLTIU)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] < (int32_t)(int16_t)(instruction.itype.imm);
            processor->PC += 4;
            break;
        case 0x0c: // opcode == 0x0c (ANDI)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] & instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0x0e: // opcode == 0x0e (XORI)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] ^ instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0x0f: // opcode == 0x0f (LUI)
            processor->R[instruction.itype.rt] = instruction.itype.imm << 16;
            processor->PC += 4;
            break;
        case 0x20: // opcode == 0x20 (LB)
            processor->R[instruction.itype.rt] = (int32_t)(int16_t)(load(memory, processor->R[instruction.itype.rs] + (int32_t)(int16_t)(instruction.itype.imm), LENGTH_BYTE));
            processor->PC += 4;
            break;
        case 0x21: // opcode == 0x21 (LH)
            processor->R[instruction.itype.rt] = (int32_t)(int16_t)(load(memory, processor->R[instruction.itype.rs] + (int32_t)(int16_t)(instruction.itype.imm), LENGTH_HALF_WORD));
            processor->PC += 4;
            break;
        case 0x23: // opcode == 0x23 (LW)
            processor->R[instruction.itype.rt] = (load(memory, processor->R[instruction.itype.rs] + (int32_t)(int16_t)(instruction.itype.imm), LENGTH_WORD));
            processor->PC += 4;
            break;
        case 0x24: // opcode == 0x24 (LBU)
            processor->R[instruction.itype.rt] = (load(memory, processor->R[instruction.itype.rs] + (int32_t)(int16_t)(instruction.itype.imm), LENGTH_BYTE));
            processor->PC += 4;
            break;
        case 0x25: // opcode == 0x25 (LHU)
            processor->R[instruction.itype.rt] = (load(memory, processor->R[instruction.itype.rs] + (int32_t)(int16_t)(instruction.itype.imm), LENGTH_HALF_WORD));
            processor->PC += 4;
            break;
        case 0x28: // opcode == 0x28 (SB)
            store(memory, processor->R[instruction.itype.rs] + (int32_t)(int16_t)(instruction.itype.imm), LENGTH_BYTE, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        case 0x29: // opcode == 0x29 (SH)
            store(memory, processor->R[instruction.itype.rs] + (int32_t)(int16_t)(instruction.itype.imm), LENGTH_HALF_WORD, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
	    case 0x2b: // opcode == 0x2b (SW)
            store(memory, processor->R[instruction.itype.rs] + (int32_t)(int16_t)(instruction.itype.imm), LENGTH_WORD, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        default: // undefined opcode
            fprintf(stderr,"%s: pc=%08x,illegal instruction: %08x\n",__FUNCTION__,processor->PC,instruction.bits);
            exit(-1);
            break;
    }
}


int check(Address address,Alignment alignment) {

    /* YOUR CODE HERE */
    if((address == 0) || ((address + alignment) < 1) || ((address + alignment) >= MEMORY_SPACE)){
        return 0;
    }
    if(alignment == LENGTH_HALF_WORD && (address % 2 != 0)) {
        return 0;
    }
    if(alignment == LENGTH_WORD && (address % 4 != 0)) {
        return 0;
    }
    return 1;
}

void store(Byte *memory,Address address,Alignment alignment,Word value) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad write=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    /* YOUR CODE HERE */
    if(alignment == LENGTH_BYTE) {
        *(Byte*)(memory+address) = value;
    }
    if(alignment == LENGTH_HALF_WORD) {
        *(Half*)(memory+address) = value;
    }
    if(alignment == LENGTH_WORD) {
        *(Word*)(memory+address) = value;
    }
}

Word load(Byte *memory,Address address,Alignment alignment) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad read=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    /* YOUR CODE HERE */
    if(alignment == LENGTH_BYTE) {
        return *(Byte*)(memory+address);
    }
    if(alignment == LENGTH_HALF_WORD) {
        return *(Half*)(memory+address);
    }
    
    // incomplete stub to let "simple" execute
    // (only handles size == SIZE_WORD correctly)
    // feel free to delete and implement your own way
    return *(Word*)(memory+address);
}
