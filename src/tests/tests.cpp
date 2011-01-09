#include <stdio.h>
#include <string>
#include <vector>

#include "../Dict.h"
#include "../datastructs.h"
#include "../Board.h"

using namespace std;

class DictTest {
    Dict *dict;
public:
    DictTest (Dict *d) : dict(d) { }
    void containsAll () {
        char tmp[20];
        FILE *fp = fopen("../../resource/sowpods.txt", "r");
        bool hasAll = true;
        while (fscanf(fp, "%s", tmp) != EOF) {
            if(!dict->contains(tmp)) {
                hasAll = false;
                break;
            }
        }
        fclose(fp);
        if (hasAll) {
            printf("PASSED: Dictionary has all the words in sowpods.txt\n");
        } else {
            printf("FAILED: Dictionary does not contain all the words in sowpods.txt\n");
        }
    }
};

class ScoreTest {
    Board bd;
    Dict &_dict;
public:
    ScoreTest (Dict &dict) : bd(dict), _dict(dict) { }
    MoveInfo getMoveFromStr (string str) {
        int y;
        string word;
        char tmp[20], c;
        if (isalpha(str[0])) {
            sscanf(str.c_str(), "%c%d %s", &c, &y, tmp);
            return MoveInfo(toupper(c)-65, y-1, HORIZONTAL, tmp, bd);
        }
        else {
            sscanf(str.c_str(), "%d%c %s", &y, &c, tmp);
            return MoveInfo(toupper(c)-65, y-1, VERTICAL, tmp, bd);
        }
    }
    void testScores () {
        FILE *fp = fopen("scores.in", "r");
        char tmp[20];
        int predicted;
        bool allswell = true;
        int index = 0, setCount = 0;
        while (fgets(tmp, 20, fp) != 0) {
            if (tmp[0] == '-') {
                bd.reset();
                index = 0;
                ++setCount;
                continue;
            }
            MoveInfo res = getMoveFromStr(tmp);
            sscanf(tmp, "%s %s %d", tmp, tmp, &predicted);
            if (res.score() != predicted) {
                printf("FAILED: calculated score does not match predicted score.\n");
                printf("Move %d of Set %d: Calculated: %d; Predicted: %d\n", index+1, setCount+1, res.score(), predicted);
                allswell = false;
            }
            vector<string> illegals;
            if (!res.isLegal()) {
                printf("Move %d of Set %d declared as illegal:", index+1, setCount+1);
                for (uint i=0; i<illegals.size(); i++)
                    printf(" %s", illegals[i].c_str());
                printf("\n");
            }
            bd.placeTiles(res);
            index++;
        }
        if (allswell) printf("PASSED: All calculated scores match predicted scores\n");
        fclose(fp);
    }
};

int main () {
    Dict dict;
    dict.load("../../resource/dawg.bin");
    DictTest dt(&dict);
    dt.containsAll();
    ScoreTest st(dict);
    st.testScores();
    return 0;
}
