#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iterator>


#include "Aircraft.h"


namespace MsfsAeroFramework {
	std::string SimVar::to_var_string() {
		std::string var_header;
		std::string index;
		if (this->type == SimVarType::NORMAL) {
			var_header = "A:";
		}
		else {
			var_header = "L:";
		}

		if (this->index != -1) {
			index = ":" + std::to_string(this->index);
		}
		else {
			index = "";
		}
		return var_header + this->name + index;
	}

} //MsfsAeroFramework