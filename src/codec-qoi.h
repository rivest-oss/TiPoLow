/*******************************************************************************
 *
 *	Project name:
 *		TiPoLow
 *	
 *	File name:
 *		codec-qoi.h
 *	
 *	Description:
 *		[TODO]
 *	
 *	Exposed:
 *		* [TODO]
 *
 ******************************************************************************/

#ifndef __tipolow__qoi__h__
#define __tipolow__qoi__h__

#include "image.h"

namespace TiPoLow {
	namespace CodecQOI {
		class Decoder : private Image {
			private:
				Image img;
				BaseIOStream *rs = nullptr;
			
			public:
				ErrorOr<void> init(BaseIOStream* rstream) {
					ErrorOr<void> e = deinit();
					rs = rstream;
				};
				
				ErrorOr<void> deinit(void) {
					ErrorOr<void> e = img.deinit();
					if(e.error) return Error { e.error };
					
					rs = nullptr;
				};
				
				// Returns (true) if there is no more data to process,
				// returns (false) otherwise.
				ErrorOr<bool> decode(void) {
				};
				
				//data_pointer
		};
	};
};

#endif

