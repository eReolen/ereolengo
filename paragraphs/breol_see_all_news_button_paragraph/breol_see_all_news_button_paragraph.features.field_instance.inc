<?php
/**
 * @file
 * breol_see_all_news_button_paragraph.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function breol_see_all_news_button_paragraph_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance:
  // 'paragraphs_item-see_all_news_button-field_news_page_button_text'.
  $field_instances['paragraphs_item-see_all_news_button-field_news_page_button_text'] = array(
    'bundle' => 'see_all_news_button',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'paragraphs_editor_preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'paragraphs_item',
    'field_name' => 'field_news_page_button_text',
    'label' => 'Knap tekst',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 0,
    ),
  );

  // Exported field_instance:
  // 'paragraphs_item-see_all_news_button-field_news_page_button_type'.
  $field_instances['paragraphs_item-see_all_news_button-field_news_page_button_type'] = array(
    'bundle' => 'see_all_news_button',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Vælg en nyhedstype der skal linkes til med knappen. ',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 0,
      ),
      'paragraphs_editor_preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'paragraphs_item',
    'field_name' => 'field_news_page_button_type',
    'label' => 'Knaptype',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Knap tekst');
  t('Knaptype');
  t('Vælg en nyhedstype der skal linkes til med knappen. ');

  return $field_instances;
}
