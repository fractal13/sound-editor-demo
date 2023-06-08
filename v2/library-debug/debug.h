#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <sstream>
#include <stdexcept>

#define D_LOCATION __FILE__ << ":" << __LINE__
#define DEBUG_INVALID(msg) { std::stringstream ss; ss << D_LOCATION << " " << msg; throw std::invalid_argument(ss.str()); }

#endif /* _DEBUG_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
