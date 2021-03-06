README
-------
AcaiBerry is a Scrabble AI with a Qt GUI interface.

There are several Scrabble AIs on the Internet Scrabble Club servers that are named -berry. AcaiBerry's name
was inspired by them, as well as by the popularity of the acai berry in spam mail.

BUILD INSTRUCTIONS
-------------------

* Run `qmake`, then do `make release && bin/Release/AcaiBerry` or `sudo make install && AcaiBerry`

* Tested against Qt 4.5 on Linux, but should build against earlier versions of Qt as well. I think it would
work on Windows too.

* `doxygen` generates the documentation.

* Type `AcaiBerry --help` for more options.

DESCRIPTION OF THE ALGORITHM
-----------------------------

* The AI is an implementation of the Appel-Jacobson algorithm.

* To quickly generate permutations of the letters on the rack, it uses a directed acyclic graph (DAG) of the
wordlist file 'sowpods.txt'. (It's a standard scrabble wordlist.) For example, the words 'CORD', 'CARD', and
'CARP' would be converted into the following graph:

<pre>
      A -- R -- P
     / \ 
    C   R -- D
     \ /
      O
</pre>

* In the example above, if the computer doesn't have the letter 'C' in its rack, it won't even bother looking
at the rest of the graph. This reduces the number of comparisons necessary.

* Now for the placing of tiles on the board. Any tile the program places while making a horizontal move must
not inadvertently create an invalid vertical word, and vice versa. For example, consider the following
board:

<pre>
        H E
    * * * *
        A
        R
        T
</pre>

* The row with the asterisk symbols indicates the one that the program will be generating moves for. We can 
see that the first and second asterisk can be filled with any letter. The third can only be filled by one
possible letter -- 'E', to form 'HEART'. The last one can be filled by quite a few more characters, but it is
still constrained by the 'E' on top.

* Before the computer generates any moves, it first calculates all these 'constraints' imposed by the tiles on
the board. Then it traverses the word-graph created earlier with the traversal pruned by these constraints.

* Referring back to the first example: if the computer knows its second character is constrained to be an 'O',
then it won't look at the top branch consisting of 'A', 'R', and 'P'.

* When all the moves for the current turn have been generated, the AI chooses the one with the highest score.

TODO
-----

* Write more comments

* Make the AI understand strategic concepts like 'rack balance' and the value of opening / closing the board.

* Turn this into a client for online Scrabble servers.
