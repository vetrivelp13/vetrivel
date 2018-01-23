<?php
/**
 * @file
 * Administrative functions to configure DrupalChat.
 */

/**
 * Callback for admin/settings/drupalchat.
 */
function drupalchat_settings_form($form, &$form_state) {
  drupal_add_js(drupal_get_path('module', 'drupalchat') . '/drupalchat.admin.js');
  global $theme_key;

  $form = array();

  $form['drupalchat_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#description' => t('All themes from inside the <em>themes</em> folder will be displayed here.'),
    '#options' => _drupalchat_load_themes(drupal_get_path('module', 'drupalchat') . '/themes', 'css'),
    '#default_value' => variable_get('drupalchat_theme', ($theme_key == 'expertusoneV2')? 'expertusone_V2' : 'expertusone'),
  );

  $form['drupalchat_notification_sound'] = array(
    '#type' => 'select',
    '#title' => t('Notification Sound'),
    '#description' => t('Select whether to play notification sound when a new message is received.'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_notification_sound', 1),
  );

  $form['drupalchat_user_picture'] = array(
    '#type' => 'select',
    '#title' => t('User Pictures'),
    '#description' => t('Select whether to show user pictures in chat.'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_user_picture', 1),
  );

  $form['drupalchat_enable_smiley'] = array(
    '#type' => 'select',
    '#title' => t('Enable Smileys'),
    '#description' => t('Select whether to show smileys.'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_enable_smiley', 2),
  );

  $form['drupalchat_log_messages'] = array(
    '#type' => 'select',
    '#title' => t('Log chat messages'),
    //'#description' => t('Select whether to log chat messages, which can be later viewed in !inbox_link.', array('!inbox_link' => l(t('message inbox'),'drupalchat/messages/inbox'))),
    '#description' => t('Select whether to log chat messages.'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_log_messages', 2),
  );

  $form['drupalchat_user_list_scrolling_setting'] = array(
    '#type' => 'select',
    '#title' => t('Chat online users list scrolling setting'),
    '#description' => t('Select online user list scroll setting.'),
    '#options' => array(0 => 1, 40 => 2, 80 => 3, 120 => 4, 160 => 5),
    '#default_value' => variable_get('drupalchat_user_list_scrolling_setting', 1),
  );
 
  $form['drupalchat_pc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Chat Moderation'),
    '#collapsible' => TRUE,
  );
  
  $form['drupalchat_pc']['drupalchat_stop_word_list'] = array(
    '#type' => 'textarea',
	'#title' => t('Stop Words (separated by comma)'),
	'#default_value' => variable_get('drupalchat_stop_word_list',
                                                    'asshole,assholes,bastard,beastial,beastiality,beastility,bestial,bestiality,bitch,' .
                                                    'bitcher,bitchers,bitches,bitchin,bitching,blowjob,blowjobs,bullshit,clit,cock,cocks,' .
                                                    'cocksuck,cocksucked,cocksucker,cocksucking,cocksucks,cum,cummer,cumming,cums,cumshot,' .
                                                    'cunillingus,cunnilingus,cunt,cuntlick,cuntlicker,cuntlicking,cunts,cyberfuc,cyberfuck,' .
                                                    'cyberfucked,cyberfucker,cyberfuckers,cyberfucking,damn,dildo,dildos,dick,dink,dinks,' .
                                                    'ejaculate,ejaculated,ejaculates,ejaculating,ejaculatings,ejaculation,fag,fagging,faggot,' .
                                                    'faggs,fagot,fagots,fags,fart,farted,farting,fartings,farts,farty,felatio,fellatio,fingerfuck,' .
                                                    'fingerfucked,fingerfucker,fingerfuckers,fingerfucking,fingerfucks,fistfuck,fistfucked,' .
                                                    'fistfucker,fistfuckers,fistfucking,fistfuckings,fistfucks,fuck,fucked,fucker,fuckers,' .
                                                    'fuckin,fucking,fuckings,fuckme,fucks,fuk,fuks,gangbang,gangbanged,gangbangs,gaysex,' .
                                                    'goddamn,hardcoresex,horniest,horny,hotsex,jism,jiz,jizm,kock,kondum,kondums,kum,kumer,' .
                                                    'kummer,kumming,kums,kunilingus,lust,lusting,mothafuck,mothafucka,mothafuckas,mothafuckaz,' .
                                                    'mothafucked,mothafucker,mothafuckers,mothafuckin,mothafucking,mothafuckings,mothafucks,' .
                                                    'motherfuck,motherfucked,motherfucker,motherfuckers,motherfuckin,motherfucking,motherfuckings,' .
                                                    'motherfucks,niger,nigger,niggers,orgasim,orgasims,orgasm,orgasms,phonesex,phuk,phuked,' .
                                                    'phuking,phukked,phukking,phuks,phuq,pis,piss,pisser,pissed,pisser,pissers,pises,pisses,' .
                                                    'pisin,pissin,pising,pissing,pisof,pissoff,porn,porno,pornography,pornos,prick,pricks,' .
                                                    'pussies,pusies,pussy,pusy,pussys,pusys,slut,sluts,smut,spunk'),
  );
  $form['drupalchat_pc']['drupalchat_use_stop_word_list'] = array(
    '#type' => 'select',
    '#title' => t('Use Stop Words to filter chat'),
    '#description' => t('Select whether to use stop words(entered above) for filtering'),
    //'#options' => array('1' => t('Don\'t filter'), '2' => t('Filter in public chatroom'), '3' => t('Filter in private chats'), '4' => t('Filter in all rooms')),
    '#options' => array(1 => t('Don\'t filter'), 3 => t('Filter')), // EXPERTUS modified
    '#default_value' => variable_get('drupalchat_use_stop_word_list', 1),
  );

  $form['drupalchat_path'] = array(
    '#type' => 'fieldset',
    '#title' => t('DrupalChat Visibility'),
    '#collapsible' => TRUE,
    //'#collapsed' => TRUE, change made by EXPERTUS to have this fieldset open by default.
  );
  $access = user_access('use PHP for settings');
  $options = array(
    BLOCK_VISIBILITY_NOTLISTED => t('All pages except those listed'),
    BLOCK_VISIBILITY_LISTED => t('Only the listed pages'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

  if (module_exists('php') && $access) {
    $options += array(BLOCK_VISIBILITY_PHP => t('Pages on which this PHP code returns <code>TRUE</code> (experts only)'));
    $title = t('Pages or PHP code');
    $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
  }
  else {
    $title = t('Pages');
  }
  $form['drupalchat_path']['drupalchat_path_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show DrupalChat on specific pages') .
             ' (<span class="drupalchat-admin-highlight">' .
                    t('Pages on which drupalchat is shown must exactly match the pages configured for nodejs module') .
             '</span>)',
    '#options' => $options,
    '#default_value' => variable_get('drupalchat_path_visibility', BLOCK_VISIBILITY_NOTLISTED),
  );
  
  $form['drupalchat_path']['drupalchat_path_pages'] = array(
    '#type' => 'textarea',
    '#title' => '<span>' . $title . '</span>',
    '#default_value' => variable_get('drupalchat_path_pages', NULL),
    '#description' => $description,
  );

  $form['drupalchat_session_check_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Session Timeout Check Timeout'),
    '#default_value' => variable_get('drupalchat_session_check_timeout', 30),
    '#size' => 10,
    '#maxlength' => 12,
    '#required' => TRUE,
    '#description' => t("The length of time, in seconds, to wait for a response from Drupal backend to a session timeout check request." .
                        " Value cannot be less than 0." .
                        " Since logging of chat messages is piggybacking session timeout check request," .
                        " the value set here should be sufficient to allow the backend to also be able to save the chat messages in the database" .
                        " when <b>Log chat messages</b> option is enabled."),
  );

  return system_settings_form($form);
}

function drupalchat_settings_form_validate($form, &$form_state) {
  if (empty($form_state['values']['drupalchat_session_check_timeout']) || $form_state['values']['drupalchat_session_check_timeout'] < 10 ) {
    form_set_error('drupalchat_session_check_timeout', t('Session Timeout Check Timeout value cannot be less than 10 seconds.'));
  }
}

function drupalchat_settings_form_submit($form, &$form_state) {
}

function _drupalchat_load_themes($outerDir, $x) {
  $dirs = array_diff(scandir($outerDir), array('.', '..'));

  $dir_array = array();
  foreach ($dirs as $d) {
    if (is_dir($outerDir . "/" . $d)) {
      if ($innerDir = _drupalchat_load_themes($outerDir . '/' . $d, $x)) {
        $dir_array[$d] = $innerDir;
      }
    }
    elseif (($x) ? preg_match('/' . $x . '$/', $d) : 1) {
      $name = _drupalchat_remove_extension($d);
      $dir_array[$name] = $name;
    }
  }
  return $dir_array;
}

function _drupalchat_remove_extension($strName) {
  $ext = strrchr($strName, '.');

  if ($ext !== false) {
    $strName = substr($strName, 0, -strlen($ext));
  }
  return $strName;
}
