<?php
require_once('xml2array.php');

class subsurface {

  function __construct($filename,$userdef1='',$userdef2='') {
    if ( !file_exists($filename) ) {
      echo "The specified file '${filename}' does not exist, aborting.\n";
      exit;
    }
    $this->data = xml2array( file_get_contents($filename), 1, 'attribute' )['divelog'];
    $this->name = pathinfo($filename,PATHINFO_FILENAME);
    $this->dir  = pathinfo($filename,PATHINFO_DIRNAME);
    $this->sitemap_file = $this->dir.DIRECTORY_SEPARATOR.$this->name.'_sites.map';
    $this->divemap_file = $this->dir.DIRECTORY_SEPARATOR.$this->name.'_dives.map';
    $this->userdef1 = $userdef1;
    $this->userdef2 = $userdef2;
  }


/* ============================================================[ divesites ]=== */
  // create a fresh site map file
  function create_sitemap() {
    $i = 1; $map = new stdClass;
    foreach ( $this->data['divesites']['site'] as $site ) {
      $map->{$site['attr']['uuid']} = (object) [ 'id'=>$i, 'name'=>$site['attr']['name'], 'water'=>'', 'type'=>'', 'rating'=>0 ];
      ++$i;
    }
    if ( file_put_contents( $this->sitemap_file, json_encode($map, JSON_PRETTY_PRINT) ) )
      return[0,"Sitemap file '".$this->sitemap_file."' created."];
    else
      return[1,"Failed to write sitemap file '".$this->sitemap_file."'!"];
  }


  // add new divesites to the end, incrementing IDs
  function update_sitemap() {
    if ( !file_exists($this->sitemap_file) ) {
      return [1,"Could not find sitemap file '".$this->sitemap_file."', nothing to update."];
    }

    $map = json_decode( file_get_contents($this->sitemap_file) );
    if ( empty($map) ) {
      return[2,"Sitemap file '".$this->sitemap_file."' seems to be empty or malformatted, update aborted."];
    }
    $i = 0;
    foreach ( (array) $map as $site ) { // find max divesite id
      if ( $site->id > $i ) $i = $site->id;
    }
    $changed = false;
    foreach ( $this->data['divesites']['site'] as $site ) {
      if ( !property_exists($map,$site['attr']['uuid']) ) {
        $map->{$site['attr']['uuid']} = (object) [ 'id'=>++$i, 'name'=>$site['attr']['name'], 'water'=>'', 'type'=>'', 'rating'=>0 ];
        $changed = true;
      }
    }
    if ($changed) {
      if ( file_put_contents( $this->sitemap_file, json_encode($map, JSON_PRETTY_PRINT) ) ) return [0,"Sitemap file '".$this->sitemap_file."' updated."];
      else return [3,"Failed to write sitemap file '".$this->sitemap_file."'!"];
    } else return [0,"No changes found, left sitemap file '".$this->sitemap_file."' untouched."];
  }


  // convert GPS coords from decimal to string (helper for export_sites)
  function gps2str($gps,$ll) {
    $arr = explode('.',$gps);
    $grad = floor(abs($gps));
    $sec  = (abs($gps) - $grad) * 3600;
    $min  = floor($sec/60);
    $secs = round($sec - ($min * 60),2);
    $str = "${grad}° ".floor($sec/60)."' ".str_replace(',','.',$secs)."'' ";
    switch($ll) {
      case "lat": if ($gps < 0) $str .= 'S'; else $str .= 'N'; break;
      case "lon": if ($gps < 0) $str .= 'W'; else $str .= 'E'; break;
    }
    return $str;
  }


  // create PDL divesite CSV
  // id;"loc";"place";"latitude";"longitude";"altitude";"depth";"water";"type";"rating";"description"
  // (water: e.g. "fresh water"; type: e.g. "open water")
  function export_sites() {
    if ( !file_exists($this->sitemap_file) ) {
      return [1,"Could not find sitemap file '".$this->sitemap_file."', please create one first."];
    }
    $map = json_decode( file_get_contents($this->sitemap_file) );
    if ( empty($map) ) {
      return [2,"Sitemap file '".$this->sitemap_file."' seems to be empty or malformatted, export aborted."];
    }

    $csv = 'id;"loc";"place";"latitude";"longitude";"altitude";"depth";"water";"type";"rating";"description"'."\n";
    foreach ( $this->data['divesites']['site'] as $site ) {
      if ( !property_exists($map,$site['attr']['uuid']) ) {
        echo "Site '".$site['attr']['name']."' (UUID '".$site['attr']['uuid']."') not found in site map, skipping.\n";
        continue;
      }
      $csv .= $map->{$site['attr']['uuid']}->id . ';';
      $tmp = explode(':',$site['attr']['name']);
      if (count($tmp) > 1) $csv .= '"'.trim($tmp[0]).'";"'.trim($tmp[1]).'";';
      else $csv .= '"";"'.trim($site['attr']['name']);
      if ( array_key_exists('gps',$site['attr']) ) { // GPS
        $tmp = explode(' ',trim($site['attr']['gps']));
        $csv .= '"'.$this->gps2str($tmp[0],'lat').'";"'.$this->gps2str($tmp[1],'lon').'";';
        if ( empty($tmp[2]) ) $csv .= '"0";';
        else $csv .= '"'.$tmp[2].'";"';
      } else $csv .= '"";"";';
      if ( property_exists($map->{$site['attr']['uuid']},'depth') ) $csv .= '"'.$map->{$site['attr']['uuid']}->depth.'";';
      else $csv .= '"";';
      //$csv .= '"";'; // depth missing/lost in Subsurface ?
      $csv .= '"'.$map->{$site['attr']['uuid']}->water.'";"'.$map->{$site['attr']['uuid']}->type.'";"'.$map->{$site['attr']['uuid']}->rating.'";';
      //$csv .= '"";"";"";'; // water, type and rating are missing/lost in Subsurface ?
      if ( array_key_exists('notes',$site) ) $csv .= '"'.trim($site['notes']['value']).'"';
      else $csv .= '""';
      $csv .= "\n";
    }
    $csv = recode_string('utf8..lat1',$csv); // PDL expects this in Latin-1 CRLF, as from ADL conduit
    if ( file_put_contents($this->dir.DIRECTORY_SEPARATOR.'divesites.csv',$csv) ) return [0,"Divesites stored in '".$this->dir.DIRECTORY_SEPARATOR.'divesites.csv'."'."];
    else return [3,"Could not write '".$this->dir.DIRECTORY_SEPARATOR.'divesites.csv'."'!"];
  }


/* ================================================================[ dives ]=== */
  // create a fresh dive map file
  function create_divemap() {
    $i = 1; $map = new stdClass;
    foreach ( $this->data['dives']['dive'] as $dive ) {
      if ( array_key_exists('description',$dive['cylinder']['attr']) ) $name = preg_replace('!\x{2113}!u','L',$dive['cylinder']['attr']['description']); // \u2113 = "script small l" unicode, used by Subsurface in names like "15L 200 bar"
      else $name ='Standard';
      $map->{$dive['attr']['number']} = (object) [
        'tank#'=>1, 'tank_id'=>1, 'tank_name'=>$name, 'tank_gas'=>'air', 'tank_type'=>'steel',
        'current'=>'', 'workload'=>'',
        'suit_id'=>1, 'suittype'=>'wet suit',
        'visibility'=>'',
        'userdef1'=>$this->userdef1, 'userdef1_val'=>'', 'userdef2'=>$this->userdef2, 'userdef2_val'=>''
      ];
      ++$i;
    }
    if ( file_put_contents( $this->divemap_file, json_encode($map, JSON_PRETTY_PRINT) ) )
      return[0,"Divemap file '".$this->divemap_file."' created."];
    else
      return[1,"Failed to write divemap file '".$this->divemap_file."'!"];
  }


  // add new dives to the end, incrementing IDs
  function update_divemap() {
    if ( !file_exists($this->divemap_file) ) {
      return [1,"Could not find divemap file '".$this->divemap_file."', nothing to update."];
    }

    $map = json_decode( file_get_contents($this->divemap_file) );
    if ( empty($map) ) {
      return [2,"Divemap file '".$this->divemap_file."' seems to be empty or malformatted, update aborted."];
    }
    $i = 0; $changed = false;
    foreach ( (array) $map as $id => $dive ) { // find max dive id
      if ( $id > $i ) $i = $id;
    }
    foreach ( $this->data['dives']['dive'] as $dive ) {
      if ( !property_exists($map,$dive['attr']['number']) ) {
        $changed = true;
        if ( array_key_exists('description',$dive['cylinder']['attr']) ) $name = preg_replace('!\x{2113}!u','L',$dive['cylinder']['attr']['description']); // \u2113 = "script small l" unicode, used by Subsurface in names like "15L 200 bar"
        else $name ='Standard';
        $map->{$dive['attr']['number']} = (object) [
          'tank#'=>1, 'tank_id'=>1, 'tank_name'=>$name, 'tank_gas'=>'air', 'tank_type'=>'steel',
          'current'=>'', 'workload'=>'',
          'suit_id'=>1, 'suittype'=>'wet suit',
          'visibility'=>'',
          'userdef1'=>$this->userdef1, 'userdef1_val'=>'', 'userdef2'=>$this->userdef2, 'userdef2_val'=>''
        ];
      }
    }
    if ( $changed ) {
      if ( file_put_contents( $this->divemap_file, json_encode($map, JSON_PRETTY_PRINT) ) ) return [0,"Divemap file '".$this->divemap_file."' updated."];
      else return [3,"Failed to write divemap file '".$this->divemap_file."'!"];
    } else return [0,"No changes found, left divemap file '".$this->divemap_file."' untouched."];
  }


  // create PDL logbook.csv
  // !!!TODO:!!! DiveProfiles
  function export_dives($min_profile_len=0) {
    // Load divemap
    if ( !file_exists($this->divemap_file) ) {
      return [1, "Could not find divemap file '".$this->divemap_file."', please create one first."];
    }
    $dmap = json_decode( file_get_contents($this->divemap_file) );
    if ( empty($dmap) ) {
      return [2, "Divemap file '".$this->divemap_file."' seems to be empty or malformatted, export aborted."];
    }

    // load sitemap (needed for location + place)
    if ( !file_exists($this->sitemap_file) ) {
      return [3, "Could not find sitemap file '".$this->sitemap_file."', nothing to update."];
    }
    $smap = json_decode( file_get_contents($this->sitemap_file) );
    if ( empty($smap) ) {
      return [4, "Sitemap file '".$this->sitemap_file."' seems to be empty or malformatted, update aborted."];
    }

    $depths = $times = []; // store array of depths and dive times for global.csv here
    $profs = 0;            // number of exported dive profiles

    // create CSV
    $csv = 'dive#;"date";"time";site_id;"location";"place";"depth";"divetime";tank#;"tank_id";"tank_name";"tank_volume";"tank_gas";'
         . '"tank_in";"tank_out";"tank_type";"buddy";"visibility";"watertemp";"airtemp";"current";"workload";suit_id;"suitname";"suittype";'
         . '"suitweight";"weight";"'.$this->userdef1.'";"userdef1";"'.$this->userdef2.'";"userdef2";"rating";"notes"'."\n";

    foreach ( $this->data['dives']['dive'] as $dive ) {
      $datum = explode('-',$dive['attr']['date']); $uhrzeit = explode(':',$dive['attr']['time']);
      $date = mktime($uhrzeit[0],$uhrzeit[1],$uhrzeit[2],$datum[1],$datum[2],$datum[0]);
      $site = explode(':',$smap->{$dive['attr']['divesiteid']}->name);
      if ( count($site) == 1 ) $site[1] = ''; // no ":", no split (TEST!)

      $csv .= $dive['attr']['number'].';"'
            . date('j F Y',$date).'";"' . date('H:i',$date).'";' // "28 March 2004";"13:35";
            . $smap->{$dive['attr']['divesiteid']}->id.';"'      // dive site id
            . trim($site[0]).'";"'.trim($site[1]).'";"';         // location + place

      if ( array_key_exists('depth',$dive['divecomputer']) ) $depth = $dive['divecomputer']['depth']['attr']['max']; // manually added dive
      else $depth = $dive['divecomputer'][count($dive['divecomputer']) -1]['depth']['attr']['max'];
      $csv .= (int) $depth .' '.explode(' ',$depth)[1];
      $depth = (int) $depth;
      if ( $depth > 0 ) $depths[] = $depth; // ignore dives with incomplete values

      $csv .= '";"'.explode(':',$dive['attr']['duration'])[0].' min";'; // divetime; PDL expects "xx min"
      if ( explode(':',$dive['attr']['duration'])[0] > 0 ) $times[] = explode(':',$dive['attr']['duration'])[0];

      $csv .= '1;"'; // always tank# 1
      $csv .= $dmap->{$dive['attr']['number']}->tank_id .'";"';
      if ( !empty($dmap->{$dive['attr']['number']}->tank_name) ) $csv .= $dmap->{$dive['attr']['number']}->tank_name;
      elseif ( array_key_exists('cylinder',$dive) && array_key_exists('description',$dive['cylinder']) && !empty($dive['cylinder']['description']) )
        $csv .= trim($dive['cylinder']['description']);
      $csv .= '";"';
      if ( array_key_exists('cylinder',$dive) && array_key_exists('size',$dive['cylinder']['attr']) ) $csv .= preg_replace('!\x{2113}!u','L',$dive['cylinder']['attr']['size']);
      $csv .= '";"'.$dmap->{$dive['attr']['number']}->tank_gas.'";"'; // !!!TODO:!!! figure out from O2/Helium fields of Subsurface
      if ( array_key_exists('cylinder',$dive) && array_key_exists('start',$dive['cylinder']['attr']) ) {
        $tmp = explode(' ',$dive['cylinder']['attr']['start']);
        $csv .= (int) $tmp[0].' '.$tmp[1];
      }
      $csv .= '";"';
      if ( array_key_exists('cylinder',$dive) && array_key_exists('end',$dive['cylinder']['attr']) ) {
        $tmp = explode(' ',$dive['cylinder']['attr']['end']);
        $csv .= (int) $tmp[0].' '.$tmp[1];
      }
      $csv .= '";"'.$dmap->{$dive['attr']['number']}->tank_type.'";"';

      $buddy = '';
      if ( array_key_exists('buddy',$dive) && !empty($dive['buddy']['value']) ) $buddy .= $dive['buddy']['value'];
      if ( array_key_exists('divemaster',$dive) && !empty($dive['divemaster']['value']) ) {
        if (strlen($buddy) > 1) $buddy .= ', '.$dive['divemaster']['value'];
        else $buddy = $dive['divemaster']['value'];
      }
      $csv .= $buddy.'";"';

      // dive conditions (visibility, temperature)
      if ( !empty($dmap->{$dive['attr']['number']}->visibility) ) $csv .= (int) $dmap->{$dive['attr']['number']}->visibility .' '.explode(' ',$dmap->{$dive['attr']['number']}->visibility)[1];
      elseif ( array_key_exists('visibility',$dive['attr']) ) $csv .= $dive['attr']['visibility']; // in Subsurface, only 0..5 stars
      $csv .= '";"';
      if ( array_key_exists('divetemperature',$dive) ) {
        if ( array_key_exists('water',$dive['divetemperature']['attr']) ) {
          $tmp = explode(' ',$dive['divetemperature']['attr']['water']);
          $csv .= round($tmp[0]). preg_replace('!C$!i',' °C',$tmp[1]);
        }
        $csv .= '";"';
        if ( array_key_exists('air',$dive['divetemperature']['attr']) ) {
          $tmp = explode(' ',$dive['divetemperature']['attr']['air']);
          $csv .= round($tmp[0]). preg_replace('!C$!i',' °C',$tmp[1]);
        }
        $csv .= '";"';
      } else $csv .='";"";"';

      // current & workload
      $csv .= $dmap->{$dive['attr']['number']}->current .'";"' . $dmap->{$dive['attr']['number']}->workload .'";';

      // suit (id, name, type, weight), weight
      $csv .= $dmap->{$dive['attr']['number']}->suit_id .';"';
      if ( array_key_exists('suit',$dive) ) $csv .= $dive['suit']['value'] .'";"';
      else $csv .'";"';
      $csv .= $dmap->{$dive['attr']['number']}->suittype .'";"";"'; // skip suit weight
      if ( array_key_exists('weightsystem',$dive) && array_key_exists('weight',$dive['weightsystem']['attr']) ) {
        $tmp = explode(' ',$dive['weightsystem']['attr']['weight']); // SubSurface also has 'description', but PDL does not
        $csv .= (int) $tmp[0].' '.$tmp[1];
      }
      $csv .= '";"';

      // userdef
      $csv .= $dmap->{$dive['attr']['number']}->userdef1 . '";"' . $dmap->{$dive['attr']['number']}->userdef1_val . '";"';
      $csv .= $dmap->{$dive['attr']['number']}->userdef2 . '";"' . $dmap->{$dive['attr']['number']}->userdef2_val . '";"';

      // rating, notes
      if ( array_key_exists('rating',$dive['attr']) ) $csv .= $dive['attr']['rating'] . '";"';
      else $csv .= '-";"';
      if ( array_key_exists('notes',$dive) ) $csv .= $dive['notes']['value'] .'"';
      else $csv .= '"';

      // finito, save logbook
      $csv .= "\n";

      // dive profile
      if ( array_key_exists('divecomputer',$dive) && array_key_exists('sample',$dive['divecomputer']) ) {
        $prof = '"time";"depth";"gas";tank#;"warning"' ."\n";
        if ( $min_profile_len > 0 && count($dive['divecomputer']['sample']) < $min_profile_len ) continue; // skip dummy profiles
        foreach ( $dive['divecomputer']['sample'] as $sample ) {
          $tmp = explode(' ',$sample['attr']['time']);
          $prof .= '"'.$tmp[0].'";"'.$sample['attr']['depth'].'";"'.$dmap->{$dive['attr']['number']}->tank_gas.'";1;""' . "\n";
        }
        $tmp = 'dive'.str_pad($dive['attr']['number'],5,'0',STR_PAD_LEFT).'_profile.csv';
        $prof = recode_string('utf8..lat1',$prof);
        if ( file_put_contents($this->dir.DIRECTORY_SEPARATOR.$tmp,$prof) ) ++$profs;
      }
    }
    $csv = recode_string('utf8..lat1',$csv); // PDL expects this in Latin-1 CRLF, as from ADL conduit
    if ( file_put_contents($this->dir.DIRECTORY_SEPARATOR.'logbook.csv',$csv) ) return [0,"Divelog stored in '".$this->dir.DIRECTORY_SEPARATOR.'logbook.csv'."'. ${profs} dive profiles exported along."];
    else return [5,"Could not write '".$this->dir.DIRECTORY_SEPARATOR.'logbook.csv'."'!"];

    // statistics (global.csv) from depths[] and times[]
    $csv = "max_depth;max_time;avg_depth;avg_time;cum_dive_time;num_dives\n";
    $cum_min = array_sum($times);
    $cum_h = floor($cum_min/60);
    $cum = "${cum_h} hours ".($cum_min - $cum_h*60).' minutes';
    $csv .= max($depths).' m;'.max($times).' min;'.round(array_sum($depths)/count($depths),1).' m;'
         .  round(array_sum($times)/count($times)).' min;'.$cum.';'.count($this->data['dives']['dive'])."\n";
    $csv = recode_string('utf8..lat1',$csv); // PDL expects this in Latin-1 CRLF, as from ADL conduit
    if ( file_put_contents($this->dir.DIRECTORY_SEPARATOR.'global.csv',$csv) ) return [0,"Statistics stored in '".$this->dir.DIRECTORY_SEPARATOR.'global.csv'."'."];
    else return [6,"Could not write '".$this->dir.DIRECTORY_SEPARATOR.'global.csv'."'!"];
  }

}
?>