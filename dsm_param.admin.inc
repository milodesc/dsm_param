<?php

/**
 * @file
 * Administration page callbacks for the Parameter Message module.
 */

/**
 * Returns an administrative page for the Parameter Message module where each
 * existing parameter message can be edited or deleted and new parameter
 * messages can be created.
 *
 * @return string
 *   A HTML-formatted string with the administrative page content.
 *
 * @see dsm_param_menu()
 */
function dsm_param_admin() {
  $header = array(t('Parameter Name'), t('Message Status'), t('Repeat Message'), t('Edit / Delete'));
  $rows = array();
  $result = db_query('select * from {dsm_param_parameters}');
  foreach($result as $parameter) {
    $rows[] = array(
      $parameter->parameter_name,
      $parameter->message_status,
      $parameter->repeat_message,
      l(t('Edit'), 'admin/config/dsm_param/edit/' . $parameter->pid) . ' | ' . l(t('Delete'), 'admin/config/dsm_param/delete/' . $parameter->pid),
    );
  }
  if (!empty($rows)) {
    $output = theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('class' => array('dsm_param_config')),
    ));
    $output .= l(t('Create a new parameter'), 'admin/config/dsm_param/create');
  } else {
    $output = t("There aren't any parameters currently set up. ") . l(t('Click here to set one up.'), 'admin/config/dsm_param/create');
  }
  return $output;
}

/**
 * Returns an administrative form for creating a Parameter Message.
 *
 * @return string
 *   A HTML-formatted string with the administrative page content.
 *
 * @see dsm_param_menu()
 */
function dsm_param_create_parameter() {
  $form = array();
  $form['dsm_param_parameter_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Parameter Name'),
    '#size' => 20,
    '#maxlength' => 64,
    '#description' => t("If a parameter with this name is in the URL, drupal will set a message for the user with the parameter's value."),
    '#required' => TRUE,
  );
  $form['dsm_param_message_status'] = array(
    '#type' => 'select',
    '#title' => t('Message Status'),
    '#default_value' => 'status',
    '#description' => t("The message's type, either status, warning or error."),
    '#options' => array(
      'status' => t('Status'),
      'warning' => t('Warning'),
      'error' => t('Error'),
    ),
    '#required' => TRUE,
  );
  $form['dsm_param_message_repeat'] = array(
    '#type' => 'radios',
    '#title' => ('Repeat Message'),
    '#default_value' => 'FALSE',
    '#description' => t("If this is set to False and the message has already been set, then the message won't be repeated."),
    '#options' => array(
      'TRUE' => t('True'),
      'FALSE' => t('False'),
    ),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Create Parameter',
  );

  return $form;
}

function dsm_param_create_parameter_validate($form, &$form_state) {
  // Check if name supplied is already used.
  $result = db_query('select parameter_name from {dsm_param_parameters}');
  foreach($result as $parameter) {
    if($form['dsm_param_parameter_name']['#value'] == $parameter->parameter_name) {
      form_set_error('dsm_param_parameter_name', 'The parameter name is already in use. Please choose another name.');
    }
  }

  // Check the validity of the Message Status field.
  $valid_statuses = array('status', 'warning', 'error');
  $submitted_status = $form['dsm_param_message_status']['#value'];
  if(!in_array($submitted_status, $valid_statuses)) {
    form_set_error('dsm_param_message_status', 'You have submitted an invalid message status. Please choose another status.');
  }

  // Check the validity of the Repeat Message field.
  $valid_repeat = array('TRUE', 'FALSE');
  $submitted_repeat = $form['dsm_param_message_repeat']['#value'];
  if(!in_array($submitted_repeat, $valid_repeat)) {
    form_set_error('dsm_param_message_repeat', 'You have submitted an invalid value for the Repeat Message field. Please choose true or false.');
  }
}

function dsm_param_create_parameter_submit($form, &$form_state) {
  db_insert('dsm_param_parameters')
    ->fields(array(
      'parameter_name' => $form['dsm_param_parameter_name']['#value'],
      'message_status' => $form['dsm_param_message_status']['#value'],
      'repeat_message' => $form['dsm_param_message_repeat']['#value'],
    ))
    ->execute();
  drupal_set_message('Your parameter has been saved.');
  drupal_goto('admin/config/dsm_param');
}
