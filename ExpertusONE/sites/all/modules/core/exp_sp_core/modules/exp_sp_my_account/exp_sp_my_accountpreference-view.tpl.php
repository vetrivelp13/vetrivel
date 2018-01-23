<?php
/**
 * @file
 * Theme implementation to display a block.
 *
 * Available variables:
 * - $title: Block title.
 * - $content: Block content.
 * - $block->module: Module that generated the block.
 * - $block->delta: An ID for the block, unique within each module.
 * - $block->region: The block region embedding the current block.
 * - $edit_links: A list of contextual links for the block. It can be
 *   manipulated through the variable $edit_links_array from preprocess
 *   functions.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - block: The current template type, i.e., "theming hook".
 *   - block-[module]: The module generating the block. For example, the user
 *     module is responsible for handling the default user navigation block. In
 *     that case the class would be "block-user".
 *   - first: The first block in the region.
 *   - last: The last block in the region.
 *   - region-count-[x]: The position of the block in the list of blocks in the
 *     current region.
 *   - region-odd: An odd-numbered block of the list of blocks in the current
 *     region.
 *   - region-even: An even-numbered block of the list of blocks in the current
 *     region.
 *   - count-[x]: The position of the block in the list of blocks on the current
 *     page.
 *   - odd: An odd-numbered block of the list of blocks on the current page.
 *   - even: An even-numbered block of the list of blocks on the current page.
 *
 * Helper variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $edit_links_array: An array of contextual links for the block.
 * - $block_zebra: Outputs 'odd' and 'even' dependent on each block region.
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $block_id: Counter dependent on each block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $block_html_id: A valid HTML ID and guaranteed unique.
 *
 * @see template_preprocess()
 * @see zen_preprocess()
 * @see template_preprocess_block()
 * @see zen_preprocess_block()
 * @see zen_process()
 */
?>
<?php
  global $theme_key;
  //Get Site default language
  $siteDefaultLanguage = getSiteDefaultLanguageCode();
  $siteLanguage = ($siteDefaultLanguage)? $siteDefaultLanguage : 'cre_sys_lng_eng';
  $preferedLanguage = ($results->person_pref_language) ? $results->person_pref_language : $siteLanguage;
  $userlanguage = getProfileListItemName($preferedLanguage);
  if($results->person_pref_currency!=''){
	$getCurrencyDetails = getSiteCurrencyCode($results->person_pref_currency);
	$my_currency=$getCurrencyDetails[0]->currency_code." ".$getCurrencyDetails[0]->currency_symbol." ".$getCurrencyDetails[0]->currency_name;
  }else{
	$my_currency ='';
  }
?>
<div class="account-preferences-view-container">
		     <table cellspacing="2" cellpadding="2" class="my-account-box myAccount">
		     	<tbody>
		      	<tr>
		     			<td class="field-title"><?php print t("LBL297")?>:</td>
		     			<td class="field-value"><?php print $results->person_timezone_name; ?></td>
		     		</tr>
		     	    <tr>
		     			<td class="field-title"><?php print t("LBL038")?>:</td>
		     			<td class="field-value"><?php print t($userlanguage)?></td>
		     		</tr>
		     		<tr>
		     			<td class="field-title"><?php print t("Location")?>:</td>
		     			<td class="field-value"><?php print $results->my_location; ?></td>
		     		</tr>
				<tr>
		     			<td class="field-title"><?php print t("LBL101")?>:</td>
		     			<td class="field-value"><?php print t($my_currency); ?></td>
		     		</tr>
		     		<!-- <tr>
		     			<td class="field-title"><?php //print t("LBL073")?>:</td>
		     			<td class="field-value"><?php //print $results->my_job_title; ?></td>
		     		</tr>   -->
		     	</tbody>
		    </table>
 </div>

<!-- /.block -->
