<h2>Title</h2>

<input name="title_text" type="text" value="<?=@$edit->content->title?>">

<input name="db_author_author/name" type="text" value="">
<input name="db_author_author/address" type="text" value="">

<h2>Text</h2>

<textarea name="html_text" rows='30' style='width:100%'><?=@$edit->content->_html?></textarea>