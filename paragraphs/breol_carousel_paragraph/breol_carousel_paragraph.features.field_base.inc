<?php

/**
 * @file
 * breol_carousel_paragraph.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function breol_carousel_paragraph_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_material_carousel_color'.
  $field_bases['field_material_carousel_color'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_material_carousel_color',
    'indexes' => array(
      'rgb' => array(
        0 => 'rgb',
      ),
    ),
    'locked' => 0,
    'module' => 'color_field',
    'settings' => array(
      'default_colors' => '#FF73D7,#9B6EFF,#FF9100,#32B4FF,#FFD2F0,#DCC3FF,#FFDCB9,#BEE6FA',
      'profile2_private' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'color_field_rgb',
  );

  return $field_bases;
}
