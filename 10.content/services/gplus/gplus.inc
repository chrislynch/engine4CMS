<?php

// Read the GPlus XML RSS and check for new items.
$gplusrss = file_get_contents('http://gplusrss.com/rss/feed/b6f35c7c9d9a1ac73eba1a6e196929ce51423a87d828b');
$gplusxml = simplexml_load_string($gplusrss);

switchto_site();
foreach($gplusxml->channel->item as $item){
    $folderpath = text_to_path($item->title,TRUE);
    $filepath = text_to_path($item->title,FALSE);
    if (!(file_exists("10.content/shares/$folderpath"))){
        mkdir("10.content/shares/$folderpath");
        $contenttitle = $item->title;
        $contenttitle = str_ireplace('<![CDATA[', '', $contenttitle);
        $contenttitle = str_ireplace(']]>', '', $contenttitle);
        
        $content = $item->description->asXML();
        $content = str_ireplace('<![CDATA[', '', $content);
        $content = str_ireplace(']]>', '', $content);
        
        $content = str_ireplace($contenttitle, '', $content);
        $content = str_ireplace('<a href', '<a rel="nofollow" href', $content);
        $content = "#$contenttitle#\n\n" . $content . " ...\n\n[{$item->link}]({$item->link})";
        file_put_contents("10.content/shares/$folderpath/$filepath.html", $content);
        print "<li>Saved new share {$item->title}</li>";
    } else {
        print "<li>Ignored old item {$item->title}</li>";
    }
}
switchto_admin();



?>