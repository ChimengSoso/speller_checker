<main class="col-md markdown-body">

<h1 id="speller">Speller</h1>

<p><strong>Be sure to read this specification in its entirety before starting so you know what to do and how to do it!</strong></p>

<p>Implement a program that spell-checks a file, a la the below, using a hash table.</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ ./speller texts/lalaland.txt
MISSPELLED WORDS

[...]
AHHHHHHHHHHHHHHHHHHHHHHHHHHHT
[...]
Shangri
[...]
fianc
[...]
Sebastian's
[...]

WORDS MISSPELLED:
WORDS IN DICTIONARY:
WORDS IN TEXT:
TIME IN load:
TIME IN check:
TIME IN size:
TIME IN unload:
TIME IN TOTAL:
</code></pre></div></div>

<h2 id="distribution">Distribution</h2>

<h3 id="downloading">Downloading</h3>

<p>Log into <a href="https://ide.cs50.io/">CS50 IDE</a> and then, in a terminal window, execute each of the below.</p>

<ol>
  <li>Execute <code class="highlighter-rouge">cd</code> to ensure that you’re in <code class="highlighter-rouge">~/</code> (i.e., your home directory).</li>
  <li>Execute <code class="highlighter-rouge">mkdir pset5</code> to make (i.e., create) a directory called <code class="highlighter-rouge">pset5</code> in your home directory.</li>
  <li>Execute <code class="highlighter-rouge">cd pset5</code> to change into (i.e., open) that directory.</li>
  <li>Execute <code class="highlighter-rouge">wget https://cdn.cs50.net/2019/fall/psets/5/speller/speller.zip</code> to download a (compressed) ZIP file with this problem’s distribution.</li>
  <li>Execute <code class="highlighter-rouge">unzip speller.zip</code> to uncompress that file.</li>
  <li>Execute <code class="highlighter-rouge">rm speller.zip</code> followed by <code class="highlighter-rouge">yes</code> or <code class="highlighter-rouge">y</code> to delete that ZIP file.</li>
  <li>Execute <code class="highlighter-rouge">ls</code>. You should see a directory called <code class="highlighter-rouge">speller</code>, which was inside of that ZIP file.</li>
  <li>Execute <code class="highlighter-rouge">cd speller</code> to change into that directory.</li>
  <li>Execute <code class="highlighter-rouge">ls</code>. You should see this problem’s distribution:</li>
</ol>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>dictionaries/  dictionary.c  dictionary.h  keys/  Makefile  speller.c  texts/
</code></pre></div></div>

<h3 id="understanding">Understanding</h3>

<p>Theoretically, on input of size <em>n</em>, an algorithm with a running time of <em>n</em> is “asymptotically equivalent,” in terms of <em>O</em>, to an algorithm with a running time of <em>2n</em>. Indeed, when describing the running time of an algorithm, we typically focus on the dominant (i.e., most impactful) term (i.e., <em>n</em> in this case, since <em>n</em> could be much larger than 2). In the real world, though, the fact of the matter is that <em>2n</em> feels twice as slow as <em>n</em>.</p>

<p>The challenge ahead of you is to implement the fastest spell checker you can! By “fastest,” though, we’re talking actual “wall-clock,” not asymptotic, time.</p>

<p>In <code class="highlighter-rouge">speller.c</code>, we’ve put together a program that’s designed to spell-check a file after loading a dictionary of words from disk into memory. That dictionary, meanwhile, is implemented in a file called <code class="highlighter-rouge">dictionary.c</code>. (It could just be implemented in <code class="highlighter-rouge">speller.c</code>, but as programs get more complex, it’s often convenient to break them into multiple files.) The prototypes for the functions therein, meanwhile, are defined not in <code class="highlighter-rouge">dictionary.c</code> itself but in <code class="highlighter-rouge">dictionary.h</code> instead. That way, both <code class="highlighter-rouge">speller.c</code> and <code class="highlighter-rouge">dictionary.c</code> can <code class="highlighter-rouge">#include</code> the file. Unfortunately, we didn’t quite get around to implementing the loading part. Or the checking part. Both (and a bit more) we leave to you! But first, a tour.</p>

<h4 id="dictionaryh"><code class="highlighter-rouge">dictionary.h</code></h4>

<p>Open up <code class="highlighter-rouge">dictionary.h</code>, and you’ll see some new syntax, including a few lines that mention <code class="highlighter-rouge">DICTIONARY_H</code>. No need to worry about those, but, if curious, those lines just ensure that, even though <code class="highlighter-rouge">dictionary.c</code> and <code class="highlighter-rouge">speller.c</code> (which you’ll see in a moment) <code class="highlighter-rouge">#include</code> this file, <code class="highlighter-rouge">clang</code> will only compile it once.</p>

<p>Next notice how we <code class="highlighter-rouge">#include</code> a file called <code class="highlighter-rouge">stdbool.h</code>. That’s the file in which <code class="highlighter-rouge">bool</code> itself is defined. You’ve not needed it before, since the CS50 Library used to <code class="highlighter-rouge">#include</code> that for you.</p>

<p>Also notice our use of <code class="highlighter-rouge">#define</code>, a “preprocessor directive” that defines a “constant” called <code class="highlighter-rouge">LENGTH</code> that has a value of <code class="highlighter-rouge">45</code>. It’s a constant in the sense that you can’t (accidentally) change it in your own code. In fact, <code class="highlighter-rouge">clang</code> will replace any mentions of <code class="highlighter-rouge">LENGTH</code> in your own code with, literally, <code class="highlighter-rouge">45</code>. In other words, it’s not a variable, just a find-and-replace trick.</p>

<p>Finally, notice the prototypes for five functions: <code class="highlighter-rouge">check</code>, <code class="highlighter-rouge">hash</code>, <code class="highlighter-rouge">load</code>, <code class="highlighter-rouge">size</code>, and <code class="highlighter-rouge">unload</code>. Notice how three of those take a pointer as an argument, per the <code class="highlighter-rouge">*</code>:</p>

<div class="language-c highlighter-rouge"><div class="highlight"><pre class="highlight"><code><span class="n">bool</span> <span class="nf">check</span><span class="p">(</span><span class="k">const</span> <span class="kt">char</span> <span class="o">*</span><span class="n">word</span><span class="p">);</span>
<span class="kt">unsigned</span> <span class="kt">int</span> <span class="nf">hash</span><span class="p">(</span><span class="k">const</span> <span class="kt">char</span> <span class="o">*</span><span class="n">word</span><span class="p">);</span>
<span class="n">bool</span> <span class="nf">load</span><span class="p">(</span><span class="k">const</span> <span class="kt">char</span> <span class="o">*</span><span class="n">dictionary</span><span class="p">);</span>
</code></pre></div></div>

<p>Recall that <code class="highlighter-rouge">char *</code> is what we used to call <code class="highlighter-rouge">string</code>. So those three prototypes are essentially just:</p>

<div class="language-c highlighter-rouge"><div class="highlight"><pre class="highlight"><code><span class="n">bool</span> <span class="nf">check</span><span class="p">(</span><span class="k">const</span> <span class="n">string</span> <span class="n">word</span><span class="p">);</span>
<span class="kt">unsigned</span> <span class="kt">int</span> <span class="nf">hash</span><span class="p">(</span><span class="k">const</span> <span class="n">string</span> <span class="n">word</span><span class="p">);</span>
<span class="n">bool</span> <span class="nf">load</span><span class="p">(</span><span class="k">const</span> <span class="n">string</span> <span class="n">dictionary</span><span class="p">);</span>
</code></pre></div></div>

<p>And <code class="highlighter-rouge">const</code>, meanwhile, just says that those strings, when passed in as arguments, must remain constant; you won’t be able to change them, accidentally or otherwise!</p>

<h4 id="dictionaryc"><code class="highlighter-rouge">dictionary.c</code></h4>

<p>Now open up <code class="highlighter-rouge">dictionary.c</code>. Notice how, atop the file, we’ve defined a <code class="highlighter-rouge">struct</code> called <code class="highlighter-rouge">node</code> that represents a node in a hash table. And we’ve declared a global pointer array, <code class="highlighter-rouge">table</code>, which will (soon) represent the hash table you will use to keep track of words in the dictionary. The array contains <code class="highlighter-rouge">N</code> node pointers, and we’ve set <code class="highlighter-rouge">N</code> equal to <code class="highlighter-rouge">1</code> for now, meaning this hash table has just 1 bucket right now. You’ll likely want to increase the number of buckets, as by changing <code class="highlighter-rouge">N</code>, to something larger!</p>

<p>Next, notice that we’ve implemented <code class="highlighter-rouge">load</code>, <code class="highlighter-rouge">hash</code>, <code class="highlighter-rouge">check</code>, <code class="highlighter-rouge">size</code>, and <code class="highlighter-rouge">unload</code>, but only barely, just enough for the code to compile. Your job, ultimately, is to re-implement those functions as cleverly as possible so that this spell checker works as advertised. And fast!</p>

<h4 id="spellerc"><code class="highlighter-rouge">speller.c</code></h4>

<p>Okay, next open up <code class="highlighter-rouge">speller.c</code> and spend some time looking over the code and comments therein. You won’t need to change anything in this file, and you don’t need to understand its entirety, but do try to get a sense of its functionality nonetheless. Notice how, by way of a function called <code class="highlighter-rouge">getrusage</code>, we’ll be “benchmarking” (i.e., timing the execution of) your implementations of <code class="highlighter-rouge">check</code>, <code class="highlighter-rouge">load</code>, <code class="highlighter-rouge">size</code>, and <code class="highlighter-rouge">unload</code>. Also notice how we go about passing <code class="highlighter-rouge">check</code>, word by word, the contents of some file to be spell-checked. Ultimately, we report each misspelling in that file along with a bunch of statistics.</p>

<p>Notice, incidentally, that we have defined the usage of <code class="highlighter-rouge">speller</code> to be</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>Usage: speller [dictionary] text
</code></pre></div></div>

<p>where <code class="highlighter-rouge">dictionary</code> is assumed to be a file containing a list of lowercase words, one per line, and <code class="highlighter-rouge">text</code> is a file to be spell-checked. As the brackets suggest, provision of <code class="highlighter-rouge">dictionary</code> is optional; if this argument is omitted, <code class="highlighter-rouge">speller</code> will use <code class="highlighter-rouge">dictionaries/large</code> by default. In other words, running</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ ./speller text
</code></pre></div></div>

<p>will be equivalent to running</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ ./speller dictionaries/large text
</code></pre></div></div>

<p>where <code class="highlighter-rouge">text</code> is the file you wish to spell-check. Suffice it to say, the former is easier to type! (Of course, <code class="highlighter-rouge">speller</code> will not be able to load any dictionaries until you implement <code class="highlighter-rouge">load</code> in <code class="highlighter-rouge">dictionary.c</code>! Until then, you’ll see <code class="highlighter-rouge">Could not load</code>.)</p>

<p>Within the default dictionary, mind you, are 143,091 words, all of which must be loaded into memory! In fact, take a peek at that file to get a sense of its structure and size. Notice that every word in that file appears in lowercase (even, for simplicity, proper nouns and acronyms). From top to bottom, the file is sorted lexicographically, with only one word per line (each of which ends with <code class="highlighter-rouge">\n</code>). No word is longer than 45 characters, and no word appears more than once. During development, you may find it helpful to provide <code class="highlighter-rouge">speller</code> with a <code class="highlighter-rouge">dictionary</code> of your own that contains far fewer words, lest you struggle to debug an otherwise enormous structure in memory. In <code class="highlighter-rouge">dictionaries/small</code> is one such dictionary. To use it, execute</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ ./speller dictionaries/small text
</code></pre></div></div>

<p>where <code class="highlighter-rouge">text</code> is the file you wish to spell-check. Don’t move on until you’re sure you understand how <code class="highlighter-rouge">speller</code> itself works!</p>

<p>Odds are, you didn’t spend enough time looking over <code class="highlighter-rouge">speller.c</code>. Go back one square and walk yourself through it again!</p>

<h4 id="texts"><code class="highlighter-rouge">texts/</code></h4>

<p>So that you can test your implementation of <code class="highlighter-rouge">speller</code>, we’ve also provided you with a whole bunch of texts, among them the script from <em>La La Land</em>, the text of the Affordable Care Act, three million bytes from Tolstoy, some excerpts from <em>The Federalist Papers</em> and Shakespeare, the entirety of the King James V Bible and the Koran, and more. So that you know what to expect, open and skim each of those files, all of which are in a directory called <code class="highlighter-rouge">texts</code> within your <code class="highlighter-rouge">pset5</code> directory.</p>

<p>Now, as you should know from having read over <code class="highlighter-rouge">speller.c</code> carefully, the output of <code class="highlighter-rouge">speller</code>, if executed with, say,</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ ./speller texts/lalaland.txt
</code></pre></div></div>

<p>will eventually resemble the below. For now, try the staff’s solution (using the default dictionary) by executing</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ ~cs50/2019/fall/pset5/speller texts/lalaland.txt
</code></pre></div></div>

<p>Below’s some of the output you’ll see. For information’s sake, we’ve excerpted some examples of “misspellings.” And lest we spoil the fun, we’ve omitted our own statistics for now.</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>MISSPELLED WORDS

[...]
AHHHHHHHHHHHHHHHHHHHHHHHHHHHT
[...]
Shangri
[...]
fianc
[...]
Sebastian's
[...]

WORDS MISSPELLED:
WORDS IN DICTIONARY:
WORDS IN TEXT:
TIME IN load:
TIME IN check:
TIME IN size:
TIME IN unload:
TIME IN TOTAL:
</code></pre></div></div>

<p><code class="highlighter-rouge">TIME IN load</code> represents the number of seconds that <code class="highlighter-rouge">speller</code> spends executing your implementation of <code class="highlighter-rouge">load</code>. <code class="highlighter-rouge">TIME IN check</code> represents the number of seconds that <code class="highlighter-rouge">speller</code> spends, in total, executing your implementation of <code class="highlighter-rouge">check</code>. <code class="highlighter-rouge">TIME IN size</code> represents the number of seconds that <code class="highlighter-rouge">speller</code> spends executing your implementation of <code class="highlighter-rouge">size</code>. <code class="highlighter-rouge">TIME IN unload</code> represents the number of seconds that <code class="highlighter-rouge">speller</code> spends executing your implementation of <code class="highlighter-rouge">unload</code>. <code class="highlighter-rouge">TIME IN TOTAL</code> is the sum of those four measurements.</p>

<p><strong>Note that these times may vary somewhat across executions of <code class="highlighter-rouge">speller</code>, depending on what else CS50 IDE is doing, even if you don’t change your code.</strong></p>

<p>Incidentally, to be clear, by “misspelled” we simply mean that some word is not in the <code class="highlighter-rouge">dictionary</code> provided.</p>

<h4 id="makefile"><code class="highlighter-rouge">Makefile</code></h4>

<p>And, lastly, recall that <code class="highlighter-rouge">make</code> automates compilation of your code so that you don’t have to execute <code class="highlighter-rouge">clang</code> manually along with a whole bunch of switches. However, as your programs grow in size, <code class="highlighter-rouge">make</code> won’t be able to infer from context anymore how to compile your code; you’ll need to start telling <code class="highlighter-rouge">make</code> how to compile your program, particularly when they involve multiple source (i.e., <code class="highlighter-rouge">.c</code>) files, as in the case of this problem. And so we’ll utilize a <code class="highlighter-rouge">Makefile</code>, a configuration file that tells <code class="highlighter-rouge">make</code> exactly what to do. Open up <code class="highlighter-rouge">Makefile</code>, and you should see four lines:</p>

<ol>
  <li>The first line tells <code class="highlighter-rouge">make</code> to execute the subsequent lines whenever you yourself execute <code class="highlighter-rouge">make speller</code> (or just <code class="highlighter-rouge">make</code>).</li>
  <li>The second line tells <code class="highlighter-rouge">make</code> how to compile <code class="highlighter-rouge">speller.c</code> into machine code (i.e., <code class="highlighter-rouge">speller.o</code>).</li>
  <li>The third line tells <code class="highlighter-rouge">make</code> how to compile <code class="highlighter-rouge">dictionary.c</code> into machine code (i.e., <code class="highlighter-rouge">dictionary.o</code>).</li>
  <li>The fourth line tells <code class="highlighter-rouge">make</code> to link <code class="highlighter-rouge">speller.o</code> and <code class="highlighter-rouge">dictionary.o</code> in a file called <code class="highlighter-rouge">speller</code>.</li>
</ol>

<p><strong>Be sure to compile <code class="highlighter-rouge">speller</code> by executing <code class="highlighter-rouge">make speller</code> (or just <code class="highlighter-rouge">make</code>). Executing <code class="highlighter-rouge">make dictionary</code> won’t work!</strong></p>

<h2 id="specification">Specification</h2>

<p>Alright, the challenge now before you is to implement, in order, <code class="highlighter-rouge">load</code>, <code class="highlighter-rouge">hash</code>, <code class="highlighter-rouge">size</code>, <code class="highlighter-rouge">check</code>, and <code class="highlighter-rouge">unload</code> as efficiently as possible using a hash table in such a way that <code class="highlighter-rouge">TIME IN load</code>, <code class="highlighter-rouge">TIME IN check</code>, <code class="highlighter-rouge">TIME IN size</code>, and <code class="highlighter-rouge">TIME IN unload</code> are all minimized. To be sure, it’s not obvious what it even means to be minimized, inasmuch as these benchmarks will certainly vary as you feed <code class="highlighter-rouge">speller</code> different values for <code class="highlighter-rouge">dictionary</code> and for <code class="highlighter-rouge">text</code>. But therein lies the challenge, if not the fun, of this problem. This problem is your chance to design. Although we invite you to minimize space, your ultimate enemy is time. But before you dive in, some specifications from us.</p>

<ul>
  <li data-marker="*">You may not alter <code class="highlighter-rouge">speller.c</code> or <code class="highlighter-rouge">Makefile</code>.</li>
  <li data-marker="*">You may alter <code class="highlighter-rouge">dictionary.c</code> (and, in fact, must in order to complete the implementations of <code class="highlighter-rouge">load</code>, <code class="highlighter-rouge">hash</code>, <code class="highlighter-rouge">size</code>, <code class="highlighter-rouge">check</code>, and <code class="highlighter-rouge">unload</code>), but you may not alter the declarations (i.e., prototypes) of <code class="highlighter-rouge">load</code>, <code class="highlighter-rouge">hash</code>, <code class="highlighter-rouge">size</code>, <code class="highlighter-rouge">check</code>, or <code class="highlighter-rouge">unload</code>. You may, though, add new functions and (local or global) variables to <code class="highlighter-rouge">dictionary.c</code>.</li>
  <li data-marker="*">You may change the value of <code class="highlighter-rouge">N</code> in <code class="highlighter-rouge">dictionary.c</code>, so that your hash table can have more buckets.</li>
  <li data-marker="*">You may alter <code class="highlighter-rouge">dictionary.h</code>, but you may not alter the declarations of <code class="highlighter-rouge">load</code>, <code class="highlighter-rouge">hash</code>, <code class="highlighter-rouge">size</code>, <code class="highlighter-rouge">check</code>, or <code class="highlighter-rouge">unload</code>.</li>
  <li data-marker="*">Your implementation of <code class="highlighter-rouge">check</code> must be case-insensitive. In other words, if <code class="highlighter-rouge">foo</code> is in dictionary, then <code class="highlighter-rouge">check</code> should return true given any capitalization thereof; none of <code class="highlighter-rouge">foo</code>, <code class="highlighter-rouge">foO</code>, <code class="highlighter-rouge">fOo</code>, <code class="highlighter-rouge">fOO</code>, <code class="highlighter-rouge">fOO</code>, <code class="highlighter-rouge">Foo</code>, <code class="highlighter-rouge">FoO</code>, <code class="highlighter-rouge">FOo</code>, and <code class="highlighter-rouge">FOO</code> should be considered misspelled.</li>
  <li data-marker="*">Capitalization aside, your implementation of <code class="highlighter-rouge">check</code> should only return <code class="highlighter-rouge">true</code> for words actually in <code class="highlighter-rouge">dictionary</code>. Beware hard-coding common words (e.g., <code class="highlighter-rouge">the</code>), lest we pass your implementation a <code class="highlighter-rouge">dictionary</code> without those same words. Moreover, the only possessives allowed are those actually in <code class="highlighter-rouge">dictionary</code>. In other words, even if <code class="highlighter-rouge">foo</code> is in <code class="highlighter-rouge">dictionary</code>, <code class="highlighter-rouge">check</code> should return <code class="highlighter-rouge">false</code> given <code class="highlighter-rouge">foo's</code> if <code class="highlighter-rouge">foo's</code> is not also in <code class="highlighter-rouge">dictionary</code>.</li>
  <li data-marker="*">You may assume that any <code class="highlighter-rouge">dictionary</code> passed to your program will be structured exactly like ours, alphabetically sorted from top to bottom with one word per line, each of which ends with <code class="highlighter-rouge">\n</code>. You may also assume that <code class="highlighter-rouge">dictionary</code> will contain at least one word, that no word will be longer than <code class="highlighter-rouge">LENGTH</code> (a constant defined in <code class="highlighter-rouge">dictionary.h</code>) characters, that no word will appear more than once, that each word will contain only lowercase alphabetical characters and possibly apostrophes, and that no word will start with an apostrophe.</li>
  <li data-marker="*">You may assume that <code class="highlighter-rouge">check</code> will only be passed words that contain (uppercase or lowercase) alphabetical characters and possibly apostrophes.</li>
  <li data-marker="*">Your spell checker may only take <code class="highlighter-rouge">text</code> and, optionally, <code class="highlighter-rouge">dictionary</code> as input. Although you might be inclined (particularly if among those more comfortable) to “pre-process” our default dictionary in order to derive an “ideal hash function” for it, you may not save the output of any such pre-processing to disk in order to load it back into memory on subsequent runs of your spell checker in order to gain an advantage.</li>
  <li data-marker="*">Your spell checker must not leak any memory. Be sure to check for leaks with <code class="highlighter-rouge">valgrind</code>.</li>
  <li data-marker="*">You may search for (good) hash functions online, so long as you cite the origin of any hash function you integrate into your own code.</li>
</ul>

<p>Alright, ready to go?</p>

<ul>
  <li data-marker="*">Implement <code class="highlighter-rouge">load</code>.</li>
  <li data-marker="*">Implement <code class="highlighter-rouge">hash</code>.</li>
  <li data-marker="*">Implement <code class="highlighter-rouge">size</code>.</li>
  <li data-marker="*">Implement <code class="highlighter-rouge">check</code>.</li>
  <li data-marker="*">Implement <code class="highlighter-rouge">unload</code>.</li>
</ul>

<h2 id="hints">Hints</h2>

<p>To compare two strings case-insensitively, you may find <a href="https://man.cs50.io/3/strcasecmp"><code class="highlighter-rouge">strcasecmp</code></a> (declared in <code class="highlighter-rouge">strings.h</code>) useful!</p>

<p>Ultimately, be sure to <code class="highlighter-rouge">free</code> in <code class="highlighter-rouge">unload</code> any memory that you allocated in <code class="highlighter-rouge">load</code>! Recall that <code class="highlighter-rouge">valgrind</code> is your newest best friend. Know that <code class="highlighter-rouge">valgrind</code> watches for leaks while your program is actually running, so be sure to provide command-line arguments if you want <code class="highlighter-rouge">valgrind</code> to analyze <code class="highlighter-rouge">speller</code> while you use a particular <code class="highlighter-rouge">dictionary</code> and/or text, as in the below. Best to use a small text, though, else <code class="highlighter-rouge">valgrind</code> could take quite a while to run.</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ valgrind ./speller texts/cat.txt
</code></pre></div></div>

<p>If you run <code class="highlighter-rouge">valgrind</code> without specifying a <code class="highlighter-rouge">text</code> for <code class="highlighter-rouge">speller</code>, your implementations of <code class="highlighter-rouge">load</code> and <code class="highlighter-rouge">unload</code> won’t actually get called (and thus analyzed).</p>

<p>If unsure how to interpret the output of <code class="highlighter-rouge">valgrind</code>, do just ask <code class="highlighter-rouge">help50</code> for help:</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ help50 valgrind ./speller texts/cat.txt
</code></pre></div></div>

<h2 id="testing">Testing</h2>

<p>How to check whether your program is outting the right misspelled words? Well, you’re welcome to consult the “answer keys” that are inside of the <code class="highlighter-rouge">keys</code> directory that’s inside of your <code class="highlighter-rouge">speller</code> directory. For instance, inside of <code class="highlighter-rouge">keys/lalaland.txt</code> are all of the words that your program <em>should</em> think are misspelled.</p>

<p>You could therefore run your program on some text in one window, as with the below.</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ ./speller texts/lalaland.txt
</code></pre></div></div>

<p>And you could then run the staff’s solution on the same text in another window, as with the below.</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ ~cs50/2019/fall/pset5/speller texts/lalaland.txt
</code></pre></div></div>

<p>And you could then compare the windows visually side by side. That could get tedious quickly, though. So you might instead want to “redirect” your program’s output to a file, as with the below.</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ ./speller texts/lalaland.txt &gt; student.txt
$ ~cs50/2019/fall/pset5/speller texts/lalaland.txt &gt; staff.txt
</code></pre></div></div>

<p>You can then compare both files side by side in the same window with a program like <code class="highlighter-rouge">diff</code>, as with the below.</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ diff -y student.txt staff.txt
</code></pre></div></div>

<p>Alternatively, to save time, you could just compare your program’s output (assuming you redirected it to, e.g., <code class="highlighter-rouge">student.txt</code>) against one of the answer keys without running the staff’s solution, as with the below.</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ diff -y student.txt keys/lalaland.txt
</code></pre></div></div>

<p>If your program’s output matches the staff’s, <code class="highlighter-rouge">diff</code> will output two columns that should be identical except for, perhaps, the running times at the bottom. If the columns differ, though, you’ll see a <code class="highlighter-rouge">&gt;</code> or <code class="highlighter-rouge">|</code> where they differ. For instance, if you see</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>MISSPELLED WORDS                                                MISSPELLED WORDS

TECHNO                                                          TECHNO
L                                                               L
                                                              &gt; Thelonious
Prius                                                           Prius
                                                              &gt; MIA
L                                                               L
</code></pre></div></div>

<p>that means your program (whose output is on the left) does not think that <code class="highlighter-rouge">Thelonious</code> or <code class="highlighter-rouge">MIA</code> is misspelled, even though the staff’s output (on the right) does, as is implied by the absence of, say, <code class="highlighter-rouge">Thelonious</code> in the lefthand column and the presence of <code class="highlighter-rouge">Thelonious</code> in the righthand column.</p>

<h3 id="check50"><code class="highlighter-rouge">check50</code></h3>

<p>To test your code less manually (though still not exhaustively), you may also execute the below.</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ check50 cs50/problems/2020/x/speller
</code></pre></div></div>

<p>Note that <code class="highlighter-rouge">check50</code> will also check for memory leaks, so be sure you’ve run <code class="highlighter-rouge">valgrind</code> as well.</p>

<h2 id="staffs-solution">Staff’s Solution</h2>

<p>How to assess just how fast (and correct) your code is? Well, as always, feel free to play with the staff’s solution, as with the below, and compare its numbers against yours.</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ ~cs50/2019/fall/pset5/speller texts/lalaland.txt
</code></pre></div></div>

<h2 id="big-board">Big Board</h2>

<p>And if you’d like to put your code to the test against classmates’ code (just for fun), execute the command below to challenge the Big Board before or after you submit.</p>

<details>
    <summary>Submit to Big Board</summary>
    <div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ submit50 cs50/problems/2020/x/challenges/speller
</code></pre></div></div>
<p>Then visit the URL that <code class="highlighter-rouge">submit50</code> outputs to see where you rank!</p>

<p><strong>Important Note: Submitting to the Big Board is not the same thing as submitting the problem set itself. To submit the problem set, complete the How to Submit instructions in the next section.</strong></p>
</details>

<h2 id="how-to-submit">How to Submit</h2>

<p>Execute the below, logging in with your GitHub username and password when prompted. For security, you’ll see asterisks (<code class="highlighter-rouge">*</code>) instead of the actual characters in your password.</p>

<div class="highlighter-rouge"><div class="highlight"><pre class="highlight"><code>$ submit50 cs50/problems/2020/x/speller
</code></pre></div></div>

</main>
