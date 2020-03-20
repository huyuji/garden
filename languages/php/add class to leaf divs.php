<?php

private function prepareNoteForPrint(?string $note): ?string {
	$dom = new DOMDocument();

	if ($dom->loadHTML($note, LIBXML_HTML_NOIMPLIED | LIBXML_HTML_NODEFDTD) !== FALSE) {
		$divs = $dom->getElementsByTagName('div');
		foreach($divs as $div) {
			$childDivs = $div->getElementsByTagName('div');
			if (count($childDivs) === 0) {
				$class = $div->getAttribute('class');
				$class .= ' avoid-page-break-inside';
				$div->setAttribute('class', $class);
			}
		}
		$note = $dom->saveHtml();
	}

	return $note;
}
