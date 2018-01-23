<?php
/**
 * @file
 * Password policy constraint callbacks.
 */

//////////////////////////////////////////////////////////////////////////////
// Constraint API

/**
 * Description of the constraint.
 */
function password_policy_constraint_length_description() {
  return array('name' => t('Length'), 'description' => t('Password must be longer than the specified minimum length.'));
}

/**
 * Error message of the constraint.
 */
function password_policy_constraint_length_error($constraint) {
  //changed for multi language by Balaji Rajendran
  return format_plural($constraint, 'Password must be at least one character in length.', t('LBL060').' '.t('MSG444').' @count '.t('MSG445'));
  //return format_plural($constraint, 'Password must be at least one character in length.', 'Password must be at least @count characters in length.');
}

/**
 * Password validation.
 */
function password_policy_constraint_length_validate($password, $constraint, $uid) {
  $length = drupal_strlen($password);
  return $length >= $constraint;
}

/**
 * Javascript portion.
 */
function password_policy_constraint_length_js($constraint, $uid) {
  $s = '';
  $s .= "  if (!value.length || value.length<$constraint) {\n";
  $s .= "    strength=\"low\";\n";
  $s .= "    msg.push(translate.constraint_length);\n";
  $s .= "  }\n";
  return $s;
}

