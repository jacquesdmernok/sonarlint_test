/**
 * @file lv_bar.h
 *
 */

#ifndef C_LV_THREATZONE_H
#define C_LV_THREATZONE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "LVGL/src/lv_conf_internal.h"
#include "LVGL/src/core/lv_obj.h"
#include "LVGL/src/misc/lv_anim.h"


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_obj_t obj;
    int32_t center_angle;       /**< The angle from the origin, measured in radians from the upward y direction */
    int32_t confidence;         /**< The span in radians of the area to be drawn, centered around the center angle*/
    lv_area_t draw_area;        /**< The area in which the zone should be drawn */
    int32_t indic_width;        /**< The thickness of the indicator*/
    lv_point_t origin;			/**< The center origin from which the zone should be drawn */
    int32_t size_offset;
} c_lv_threatzone_t;

extern const lv_obj_class_t c_lv_threatzone_class;

/**
 * `type` field in `lv_obj_draw_part_dsc_t` if `class_p = lv_bar_class`
 * Used in `LV_EVENT_DRAW_PART_BEGIN` and `LV_EVENT_DRAW_PART_END`
 */
typedef enum {
    C_LV_THREATZONE_DRAW_PART_INDICATOR,    /**< The indicator*/
} c_lv_threatzone_draw_part_type_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a bar object
 * @param parent    	pointer to an object, it will be the parent of the new bar
 * @return          	pointer to the created threatzone
 */
lv_obj_t * c_lv_threatzone_create(lv_obj_t * parent);
void c_lv_threatzone_calculate_area(c_lv_threatzone_t* threat);
void c_lv_threatzone_calculate_position(c_lv_threatzone_t* threat);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set a new angle for the threatzone
 * @param obj       pointer to a threatzone object
 * @param angle     angle in radians * 10, measured from the upward y direction
 */
void c_lv_threatzone_set_angle(lv_obj_t * obj, int32_t angle);

/**
 * Set a new draw area to which drawing should be restricted
 * @param obj       	pointer to a threatzone object
 * @param draw_area    	the area in which the widget is restricted (could be smaller than parent, but must be fully within parent)
 */
void c_lv_threatzone_set_draw_area(lv_obj_t * obj, lv_area_t draw_area);

/**
 * Set a new origin from which to extend the threatzone
 * @param obj       	pointer to a threatzone object
 * @param origin    	the origin of the threatzone (from which it extends outward)
 */
void c_lv_threatzone_set_origin(lv_obj_t * obj, lv_point_t origin);

/**
 * Set a new size offset for the outer edge of the zone
 * @param obj       	pointer to a threatzone object
 * @param origin    	New offset in Px.
 */
void c_lv_threatzone_set_size_offset(lv_obj_t * obj, int16_t offset);

/**
 * Set a new thickness for the indicator
 * @param obj       	pointer to a threatzone object
 * @param thickness    	the thickness (in px.) of the highlighted outer rim
 */
void c_lv_threatzone_set_thickness(lv_obj_t * obj, int32_t thickness);

/**
 * Set confidence angle for the threat
 * @param obj       	pointer to a threatzone object
 * @param thickness    	the confidence in radians * 10
 */
void c_lv_threatzone_set_confidence(lv_obj_t * obj, int32_t confidence);

/*=====================
 * Getter functions
 *====================*/

/**
 * Gets the current angle for the threatzone
 * @param obj       pointer to a threatzone object
 * @return    		angle in radians  * 10, measured from the upward y direction
 */
int32_t c_lv_threatzone_get_angle(const lv_obj_t * obj);

/**
 * Gets the restricted drawing area of the threatzone
 * @param obj       	pointer to a threatzone object
 * @return     			the area in which the widget is restricted (could be smaller than parent, but must be fully within parent)
 */
lv_area_t c_lv_threatzone_get_draw_area(const lv_obj_t * obj);

/**
 * Gets the current origin from which the zone extends
 * @param obj       	pointer to a threatzone object
 * @return     			the origin of the threatzone (from which it extends outward)
 */
lv_point_t c_lv_threatzone_get_origin(const lv_obj_t * obj);

/**
 * Set a new thickness for the indicator
 * @param obj       	pointer to a threatzone object
 * @return     			the thickness (in px.) of the highlighted outer rim
 */
int32_t c_lv_threatzone_get_thickness(const lv_obj_t * obj);

/**
 * Get the current confidence for the threat zone
 * @param obj       	pointer to a threatzone object
 * @return     			the thickness (in px.) of the highlighted outer rim
 */
int32_t c_lv_threatzone_get_confidence(const lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*C_LV_THREATZONE_H*/


