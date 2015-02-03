<?php

/**
 * @file
 * Administration page callbacks for the Parameter Message module.
 */

/**
 * Returns an administrative form for the Parameter Message module.
 *
 * @return string
 *   A HTML-formatted string with the administrative page content.
 *
 * @see dsm_param_menu()
 */
function dsm_param_admin() {
  $form = array();
  $form['dsm_param_parameter_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Parameter Name'),
    '#default_value' => variable_get('dsm_param_parameter_name', 'dsm_param_name'),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t("If a parameter with this name is in the URL, drupal will set a message for the user with the parameter's value"),
  );
  $form['dsm_param_parameter_status'] = array(
    '#type' => 'select',
    '#title' => t('Message Status'),
    '#default_value' => variable_get('dsm_param_parameter_status', 'status'),
    '#description' => t("The message's type, either status, warning or error."),
    '#options' => array(
      'status' => t('Status'),
      'warning' => t('Warning'),
      'error' => t('Error'),
    ),
  );
  $form['dsm_param_parameter_repeat'] = array(
    '#type' => 'radios',
    '#title' => ('Repeat Message'),
    '#default_value' => variable_get('dsm_param_parameter_repeat', 'FALSE'),
    '#description' => t("If this is set to False and the message has already been set, then the message won't be repeated."),
    '#options' => array(
      'TRUE' => t('True'),
      'FALSE' => t('False'),
    ),
  );

  return system_settings_form($form);
}
