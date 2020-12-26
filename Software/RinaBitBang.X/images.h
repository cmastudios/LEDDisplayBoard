/* 
 * File:   images.h
 * Author: connor
 *
 * Created on December 25, 2020, 6:28 PM
 */

#ifndef IMAGES_H
#define	IMAGES_H

#include <stdint.h>

struct image {
	uint8_t eyes[20 * 8 * 3];
	uint8_t mouth[10 * 6 * 3];
};

extern const struct image image_face, image_cse467, image_washu, image_error_battery, image_error_wireless;



#endif	/* IMAGES_H */

