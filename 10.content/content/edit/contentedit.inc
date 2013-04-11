<h1>Edit Content</h1>

<?php

admin_content_edit_save();
print admin_content_edit($this);

function admin_content_edit(&$e){
    
    // Open up the item that we are editing
    switchto_site();
    if (isset($_GET['directory']) && strlen($_GET['directory']) > 0){
        $edits = e::_search($_GET['directory']);
    } else {
        $edits = array();
        $edits[] = new stdClass();
    }
    switchto_admin();
    
    // Open up the template that we are working on
    foreach($edits as $edit){
        if (!isset($_GET['template'])) { $_GET['template'] = 'plaintext'; }
        ob_start();
        include('10.content/content/edit/_templates/' . $_GET['template'] . '.inc');
        $template = ob_get_contents();
        ob_end_clean();
    
        $return = "<form action='content/edit?save=save&template={$_GET['template']}&directory={$_GET['directory']}&debug' method='POST'>";
        $return .= $template;
        $return .= "<input name='save' type='submit' value='Save Content'>";
        $return .= "</form>";
    }
    
    return $return;
}

function admin_content_edit_save(){
    
    $data = array();
    
    if(isset($_GET['save'])){
        foreach($_POST as $key=>$value){
            if(strstr($key,'_')){
                $key = explode('_',$key);
                $filetype = $key[0];
                $format = $key[1];
                if (isset($key[2])){ $field = $key[2];} else { $field = ''; }
                    if(isset($data[$filetype])){
                        $data[$filetype][$field] = $value;
                    } else {
                        if (strlen($field) > 0){
                            $data[$filetype] = array();
                            $data[$filetype][$field] = $value;
                        } else {
                            $data[$filetype] = $value;
                        }
                    }
                
            }
        }
    }
    
    $directoryroot = e::_dirup(dirname($_SERVER['SCRIPT_FILENAME']));
    
    if (isset($_GET['directory']) AND strlen($_GET['directory']) > 0){
        $filename = explode('/',$_GET['directory']);
        $filename = array_pop($filename);
        $filename = str_ireplace('.', '-', $filename);
        $filename = $directoryroot . '/' . $_GET['directory'] . '/' . $filename ;
    } else {
        if (isset($data['title'])){
            // Turn the title into a filename
            $filename = $directoryroot . '/10.content/posts/' . date('YmdHi') . '.';
            $filename .= str_ireplace(' ', '-', $data['title']);
            mkdir($filename);
            $filename .= '/' . str_ireplace(' ', '-', $data['title']);
        } else {
            
        }
    }
    
    $return = TRUE;
    foreach($data as $key=>$value){
        if(is_array($value)){
            // TODO: Deal with the saving of fields
        } else {
            // We need to delete files with the $key that we are using, because there may be incorrectly named files
            $mask = "$directoryroot/{$_GET['directory']}/*.$key";
            array_map( "unlink", glob( $mask ) );
            
            $outputfilename = $filename . '.' . $key;
            if (file_put_contents($outputfilename, $value)){
                print "<strong>Success:</strong> Saved " . $outputfilename . '<br>';
            } else {
                print "<strong>Error:</strong> Unable to save " . $outputfilename . '<br>';
                $return = FALSE;
            }
        }
    }
    
    return $return;
}

?>