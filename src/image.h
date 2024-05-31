/*******************************************************************************
 *
 *	Project name:
 *		TiPoLow
 *	
 *	File name:
 *		image.h
 *	
 *	Description:
 *		Base class for raster images.
 *	
 *	Exposed:
 *		* [TODO]
 *
 ******************************************************************************/

#ifndef __tipolow__image__h__
#define __tipolow__image__h__

#include "nuclei.h"

namespace TiPoLow {
	class Image {
		private:
			// Image's width and height (in pixels).
			u32 img_w = 0, img_h = 0;
			
			// Big-Endian RGBA32.
			u8 *pix_ptr = nullptr;
		
		public:
			Image(u32 w, u32 h) { width(w); height(h); };
			Image(void) {};
			
			void width(u32 w) { img_w = w; };
			void height(u32 h) { img_h = h; };
			
			u32 width(void) { return img_w; };
			u32 height(void) { return img_h; };
			
			ErrorOr<void> init(void) {
				if(pix_ptr != nullptr) {
					ErrorOr<void> e = deinit();
					return Error { e.error };
				}
				
				if(img_w)
					return Error { "The image width is zero" };
				if(img_h)
					return Error { "The image height is zero" };
				
				ErrorOr<u8*> ptr = alloc_type<u8>((img_w * img_h) << 2);
				if(ptr.error) return Error { ptr.error };
				
				pix_ptr = ptr.value();
				
				return Error { nullptr };
			};
			
			ErrorOr<void> deinit(void) {
				if(pix_ptr == nullptr) return Error { nullptr };
				
				ErrorOr<void> e = free_type(pix_ptr);
				
				return Error { e.error };
			};
			
			u8 *data_pointer(void) { return pix_ptr; };
	};
};

#endif

