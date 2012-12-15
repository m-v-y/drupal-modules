<?php

/**
 * views_style_autoload settings form.
 *
 * @param $form
 * @param $form_state
 * @return array
 */
function views_style_autoload_settings($form, $form_state) {
  // Show debug information.
  $show_debug_information = variable_get('views_style_autoload_settings_debug', 0);
  $form['show_debug'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display which views used on current page.'),
    '#default_value' => $show_debug_information,
  );

  // Custom files path.
  $files_path = variable_get('views_style_autoload_files_path', '');
  if (empty($files_path)) {
    $default_theme = variable_get('theme_default', '');
    $theme_path = drupal_get_path('theme', $default_theme);
    $files_path = $theme_path . '/css/views-style-autoload';
  }
  $form['files_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Styles path'),
    '#default_value' => $files_path,
  );

  // List of excluded views.
  $excluded_views = variable_get('views_style_autoload_settings_excl_views', '');
  if (!empty($excluded_views)) {
    $excluded_views = implode("\r\n", $excluded_views);
  }
  $form['excluded_views'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Excluded views'),
    '#description'   => 'Input excluded views names. One per line.',
    '#default_value' => $excluded_views,
  );


  // Submit.
  $form['submit'] = array(
    '#type'        => 'submit',
    '#value'       => t('Save'),
  );

  return $form;
}

function views_style_autoload_settings_submit($form, $form_state) {
  if (isset($form_state['values']['show_debug'])) {
    variable_set('views_style_autoload_settings_debug', $form_state['values']['show_debug']);
  }

  if (!empty($form_state['values']['files_path'])) {
    variable_set('views_style_autoload_files_path', $form_state['values']['files_path']);
  }

  if (!empty($form_state['values']['excluded_views'])) {
    $excluede_views = $form_state['values']['excluded_views'];
    $excluede_views = filter_xss($excluede_views);
    $excluede_views = explode("\r\n", $excluede_views);
    variable_set('views_style_autoload_settings_excl_views', $excluede_views);
  }
}