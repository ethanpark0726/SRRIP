#ifndef RRIP_REPL_H_
#define RRIP_REPL_H_

#include "repl_policies.h"

class SRRIPReplPolicy : public ReplPolicy {
    protected:
        // add class member variables here
    	uint32_t* array;
		uint32_t numLines;
		uint32_t rpvMax;
    	uint32_t replacingIdx;
    public:
        // add member methods here, refer to repl_policies.h
	explicit SRRIPReplPolicy(uint32_t _numLines, uint32_t _rpvMax) : numLines(_numLines), rpvMax(_rpvMax), replacingIdx(_rpvMax) {
		array = gm_calloc<uint32_t>(numLines);
	}
	~SRRIPReplPolicy() {
		gm_free(array);
	}

	void update(uint32_t id, const MemReq* req) {

		// postinsert calls update (MISS and then find the best ID)
		if(id == replacingIdx) {
			array[id] = rpvMax - 1;
		}
		// lookup calls update (HIT)
		else {
			array[id] = 0;
		}
	}

	void replaced(uint32_t id) {
		replacingIdx = id;
	}

	template <typename C> inline uint32_t rank(const MemReq* req, C cands) {
		uint32_t index = -1;
		bool flag = false;

		while(index == -1) {
			for (auto ci = cands.begin(); ci != cands.end(); ci.inc()) {
				if(array[*ci] == rpvMax) {
					index = *ci;
					flag = true;
					break;
				}
			}
			if(flag == false) {
				for(auto ci = cands.begin(); ci != cands.end(); ci.inc()) {
					array[*ci] += 1;
				}
			}	
		}
		return index;
	}
        DECL_RANK_BINDINGS;
};
#endif // RRIP_REPL_H_