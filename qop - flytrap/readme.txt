The backstory:
--------------

This demo started as an idea back in 2007. First thing I did, as usual, was the soundtrack. It's been a while and I don't remeber
all the details, but can say for sure I was in a weird/dark place personally, the local scene was in shambles, there was no
FlashParty that year (and for many years after) so it got abandoned. 

A couple of years passed and (around 2010) I stumbled uppon a mixdown of the track and got inspired again, started sketching a 
story board and at some point wrote the script, then it got sidetracked again. 

Fast forward to 2014 and the local scene started showing vital signs again, the PVM and 779 guys (uctumi, octavo, arleka, acidbrain) 
and even my pals from the infosec industry started experimenting with C64's, writing shaders, and generally releasing new stuff. 

At this point I got inspired again, but since I either coudn't find or open the original Cubase file I decided to remix the soundtrack.
After that I started looking around for a rendering engine, tried everything I could get my hands on, but I either didn't 
like it (unity, unigine, leadwerks), didn't have time to learn (unreal, filament, lumberyard), or didn't want to put in the effort.

Now 2018 comes, and out of the fucking blue: FlashParty is back! I had no engine, no art and no time :(
So I made a craptro anyway... :)

2019 comes, still no engine, but a little more time. So I just dusted off my 15 year old OpenGL fixed pipeline engine, 
some old models I had from Polper and myself, a new soundtrack by Rave-n and *bam* we at least had something to release.

Fast forward to march 2020, the world is on fucking lockdown, government doesn't allow me to go BMXing as I would usually do
in my spare time, I found a cool rendering engine I can use, AND... I get word that FlashParty is a GO for mid July :)

Music:
------
The soundtrack used is the 2014 remix, keeping with the original it also abuses a Z3tA+ preset, made the drums sound puchier, 
gave it a sharper bassline and tryed to keep with the dark "spacey" atmosphere of the original, just less convoluted.
Probably one of the first tracks I did after I switched to Presonus Studio One [1].

Both versions are available for free listen/download here:
https://soundcloud.com/phienex/sets/qop-flytrap

Models/graphics:
----------------
4k space cubemap was generated with: https://wwwtyro.github.io/space-3d/
The mothership model consist mainly a couple of boxes and a bunch of kitbashing from sets downloaded from sketchfab[2], 
retopoed and textured it on 3dcoat.

The 3 legged mosquito mech basemesh I did from scratch on 3dsmax, did UVs on 3d coat, and made displacement/normal/ambient
occlusion and albedo maps on mudbox. Also added a bunch "greeble" thanks to free alpha sets downloaded from gumroad[3] and
badKing [4]. 
Then rigged it in 3dsmax with a CAT rig.

The mech pilot started as Mudbox's humanBody preset and did all the maps there as well, then rigged it in 3dsmax with a CAT rig.

Code:
-----
The new demoengine uses "Wicked Engine" as the renderer[5], a custom FBX loader/player relying heavily on FBXSDK2020.0.1[6], and Fmod
for music and 3d sound playback. All in I've put roughly 4.5k lines of "meh" quality C++ code.

Also reused my 15+ year old sync tool that let's me extract "markers" put on a wav file (in soundforge!!) and set them as Fmod callbacks to 
trigger scene/camera/vfx/dsp changes when reached :)

links:
[1] https://www.presonus.com/products/studio-one/
[2] https://sketchfab.com/distance880
[3] https://gumroad.com/d/57b06ebc6c8cc0d58f89d5c5d823d651
[4] https://www.badking.com.au/site/product-category/alpha-maps/industrial-alpha-maps/panels/
[5] https://github.com/turanszkij/WickedEngine
[6] https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-0


