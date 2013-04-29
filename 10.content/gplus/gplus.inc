<?php

// Read the GPlus XML RSS and check for new items.
$gplusrss = file_get_contents('http://gplusrss.com/rss/feed/b6f35c7c9d9a1ac73eba1a6e196929ce51423a87d828b');
$gplusxml = simplexml_load_string($gplusrss);

switchto_site();
foreach($gplusxml->channel->item as $item){
    $path = text_to_path($item->title);
    if (!(file_exists("10.content/shares/$path"))){
        mkdir("10.content/shares/$path");
        $content = "#$item->title\n\n" . $item->description->asXML() . "\n\n[{$item->link}]({$item->link})";
        file_put_contents("10.content/shares/$path/$path.html", $content);
        print "<li>Saved new share {$item->title}</li>";
    } else {
        print "<li>Ignored old item {$item->title}</li>";
    }
}
switchto_admin();



?>