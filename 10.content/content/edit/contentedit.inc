<h1>Edit Content</h1>

<?php

admin_content_edit_save($this);
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

function admin_content_edit_save(&$e){
    
    $data = array();
    
    // Collect up the data from the $_POST array()
    if(isset($_GET['save'])){
        foreach($_POST as $key=>$value){
            
            $key = explode('_',$key);
            $filetype = $key[0];
            $format = $key[1];
            if (isset($key[2])){ $field = $key[2];} else { $field = ''; }

            if (!isset($data[$filetype]) && strlen($field) > 0){
                $data[$filetype] = array('value' => array());   
            }

            if(strlen($field) > 0){
                $data[$filetype]['value'][$field] = array('format' => $format, 'value' => $value);
            } else {
                $data[$filetype] = array('format' => $format, 'value' => $value);
            }

        }
    }
    
    // Work out where the data is going to go
    $directoryroot = e::_dirup(dirname($_SERVER['SCRIPT_FILENAME']));
    
    // Check to see if this is existing data or if we are creating new data
    if (isset($_GET['directory']) AND strlen($_GET['directory']) > 0){
        $filename = explode('/',$_GET['directory']);
        $filename = array_pop($filename);
        $filename = str_ireplace('.', '-', $filename); // Replace the . with - because we need . later to make file extensions work
        $filepath = $directoryroot . '/' . $_GET['directory'];
        $filename = $directoryroot . '/' . $_GET['directory'] . '/' . $filename ;
    } else {
        // Turn the title into a filename
        if (!isset($data['title']['value'])){
            // Dumb enough to enter no title? We'll use blank
            $data['title'] = array('format' => 'text', 'value' => '');
        }
        // Build the directory structure that we need
        $filename = $directoryroot . '/10.content/posts/' . date('YmdHi') . '.';
        $filename .= e::_textToPath($data['title']['value']);
        mkdir($filename);
        $filepath = $filename;
        $filename .= '/' . e::_textToPath($data['title']['value']);
    }
    // $filepath = The path to the directory that we are in, useful when working with db type data
    // $filename = The path to any file in that directory, minus its .extension
    
    $return = TRUE;
    foreach($data as $filetype=>$value){
        switch($filetype){
            case 'db':
                // Special handler for saving information to the database.
                /*
                 * TODO: Decide if we even NEED this. What if we put the data in a file, like we should,
                 * and then indexed it automatically. Wouldn't that be better?
                 * 
                 * What is the value of something that is only in the database and we therefore need a non-e4 handler
                 * to load later? Doesn't seem to make much sense now!
                 */
                break;
            default:
                // Standard handler for everything else.
                // The standard handler saves to files
                
                // If the file already exists, we delete it.
                // We leave behind files that we do not understand in case they belong to something else.
                if(isset($_GET['directory'])){
                    $mask = "$directoryroot/{$_GET['directory']}/*.$key";
                    array_map("unlink",glob($mask));
                }
                if(is_array($value['value'])){
                    // This value has fields. We need to save it in an XML format.
                    // TODO: Because we can't do this yet!
                } else {
                    // This value does not have fields.
                    
                }
        }
        
        if(is_array($value)){
            if ($key == 'db'){
                // This data is being saved to the database
                // The standard format for saving data is powered by the index plugin
                $e->_loadPlugin('index');
                $e->_index->saveToIndex($filename,$value,$e);
            } else {
                // This data is being saved in XML format.
            }
        } else {
            // We need to delete files with the $key that we are using, because there may be incorrectly named files
            
            
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