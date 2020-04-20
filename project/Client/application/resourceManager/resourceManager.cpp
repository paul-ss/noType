#include "resourceManager.hpp"

T* GetResource(const std::string& l_id){ auto res = Find(l_id);
return(res ? res->first : nullptr);
}
