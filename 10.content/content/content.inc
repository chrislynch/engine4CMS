<h1>Your Posts</h1>
<table class="DataTable" width="100%">
    <tr>
        <th>Date</th>
        <th>Title</th>
        <th>&nbsp;</th>
        <th>&nbsp;</th>
        <th>&nbsp;</th>
    </tr>
    <?php _admin_content_listdir('10.content/posts');?>
    
</table>

<?php

function _admin_content_listdir($dir){
    switchto_site();

    $things = scandir($dir);
    $things = array_reverse($things);
    foreach($things as $thing){
        if (e::_isValidDirectory("$dir/$thing")){
            $thinga = explode('.',$thing);
            $thinga[1] = ucwords(str_ireplace('-', ' ', $thinga[1]));
            print "<tr><td>{$thinga[0]}&nbsp;</td><td>{$thinga[1]}</td>
                    <td><a href='content/edit?directory=$dir/$thing'>Edit</a></td>
                    <td>&nbsp;</td>
                    <td>Delete</td></tr>";
        }
    }

    switchto_admin();
}

?>