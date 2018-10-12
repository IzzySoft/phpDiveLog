## Subsurface Convertor
This directory holds a simple tool to import your divelogs from [Subsurface](https://subsurface-divelog.org/).
The convertor is not only far from being perfect, it currently is also incomplete. It works for simple
dive logs, but e.g. doesn't cater for multi-tank dives, multiple dive computers, or using Trimix, for example.

(For those who don't know: Subsurface is an open source divelog program for recreational, tech, and free-divers
that runs on Windows, Mac and Linux.)

Subsurface provides several fields unknown to AquaDiveLog (ADL) / phpDiveLog (PDL), like „suitweight“,
geo-categories (eg. Country), what kind of weight system (eg. „integrated“) or dive computer was used –
which this convertor ignores. But then there are several fields in ADL/PDL which Subsurface doesn't
know – but especially those having used ADL/PDL in the past don't want to lose. Moreover, Subsurface
uses „HEX“ values for divesite IDs while ADL/PDL use integers. For those things, "Subvert" uses
„mapping files“ you can modify: match the correct divesite IDs, provide additional details.


### Recommended Usage
It is recommended to start with a separate, empty directory in which a copy of Subsurface's XML file is
placed. Copy/symlink Subvert's files here (or place them into your `$PATH` – or use them from where
they are and specify the `/full/path/to/your.xml` via the `-f` parameter), then you're ready to go.
First you need to create above mentioned mapping files:

    ./subvert.php -f divelog.xml -1 Accessories create

(Of course replace `divelog.xml` with the name of your XML file). This will create two files in the
directory `divelog.xml` resides: `divelog.sites.map` and `divelog_dives.map`, containing the data
Subsurface doesn't know, so they can be merged in. Data are prepared with default values – so after
creating those maps you want to edit and adjust them with the values from your dives previously
logged using ADL/PDL. Later, when you've added more dives in Subsurface, you can easily append those
to the map files with

    ./subvert.php -f divelog.xml -1 Accessories update

After that, you only need to adjust the new records. That done, you can create the CSV files for PDL:

    ./subvert.php -f divelog.xml -1 Accessories export

Which will generate the three files `divesites.csv`, `logbook.csv` and `global.csv` along with (if the
corresponding data does exist) a bundle of `dive*_profile.csv` files (see below) – which in a final
step, you copy into your diver's PDL `data/` directory (please make a backup of existing `*.csv` files
first, just in case: Subvert is far from having been thoroughly tested, things might go wrong).


### Dive Profiles
Subvert also generates the dive profile `*.csv` files from your Subversion XML. Keep in mind that
Subversion also creates a „dummy profile“ if you manually add a dive and don't have profile data
collected with any dive computer (or by other means). You probably don't want to copy such dummies
to your PDL dive log. They are easy to spot, though, usually being smaller than 200 bytes while a
„real profile“ rarely is smaller than 2 kbytes (unless it was a very short dive).

So it's up to you which profiles you „copy over”. You can even tell `subvert.php` to not export them
in the first place, using the `-i` option: to my experience, they always have exactly 4 records, so
with `-i 5` they shouldn't be exported at all.


### Syntax
Just call `./subvert.php` without any parameters, and it will tell you it

* requires `-f` followed by the (path and) name of your XML file as first parameter
* optionally takes the parameters `-1` and `-2` to define the `userdef1` and `userdef2` columns you might have used with ADL/PDL
* knows the commands `create`, `update` (for the mapping files) and `export` – each with an optional parameter which can be
  either `dives` or `sites` (if not specified, both will be dealt with)


### Open Issues
* Subvert isn't really "fool proof". It doesn't safe-guard against all possible mistakes or errors.
* Subvert might throw some errors here and there. That is because it's not (yet) thoroughly
  tested (I had only my own data to play with). If you encounter any of those, please report them
  in the issue tracker. If you can provide me with your XML file (and maybe also with the map
  files you're using), I can see to fix them up. Or, if you do so yourself, open a PR.

