#ifndef ANTOK_PLOT_HPP
#define ANTOK_PLOT_HPP

namespace antok {

	class Plot {

	  public:

		virtual void fill(long cutmask) = 0;
		virtual ~Plot() { };

	};

}

#endif

