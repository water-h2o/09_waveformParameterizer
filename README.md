# :~ Waveform Paraturizer ~:

## Requirements

C++17, Eigen 3, CERN ROOT

---

## 1. Description

This application performs feature extraction on inputted ANTS2 waveforms. It's built so that it's easy to include new features using the extracted parameters as a base. It is also built to include functionality for categorization based on the parameters.

## 2. Basic usage

Paste all the waveforms on which you want to perform feature extraction into `.resources/input`, then run "`$ ./test`". The output data tables are .txt files saved to `.resources/output`

## 3. Other Common Use Cases

### 3.1. Setting whether you want categorization or not

At the end of `main()` there are calls to two functions, `PFwithCategories()` and `PFsansCategories()`. The names are self-explanatory. Comment out the one you don't need.

### 3.2. Adding new features

* **in `myTypedefs.h`**
    * Update the `Feature` definition to include the new feature
* **in *parameterFeaturizer* **
    * Add the the feature to `getFeatures()`
    * Decide whether the feature will be included on both ***hasCat*** and ***nonCat*** or just one of them, and update `getFeaturesNamesAbbrev_hasCat()`, `getFeatureRepetitions_hasCat()` and `getFeaturesNamesAbbrev_nonCat()`, `getFeatureRepetitions_nonCat()` appropriately
* **in *main.cpp* **
    * For the ***hasCat*** case, you need to update `writeFileHeaderTree()` if appropriate
* **in *fileOutput* **
    * Update `a_Features2a_sentence_hasCat()` and `a_Features2a_sentence_nonCat()`

## 4. Debugging

If something goes wrong *(it probably will at some point)*, you're sorta on your own **:c** . The console output is kinda spartan by default because I don't want to spam the user with information they don't care about. However if you want the console output to be more verbose, `main()` has a `verbosity` variable that can help. By setting that variable to a higher number, the program will start outputting more useful text. That way you can figure exactly on which step you're getting your segmentation fault again.

### 4.1. Some tips

*Pay attention to whether the program throws an error for every input file or only for some. If it's for every file, then that's easier to fix, as it means that probably some part of the program is expecting to be given a certain value and is being given another one instead. Just keep tracking the variable values with `cout`s until you find the culprit. If it's only for certain input files, check if there is any pattern about what files throw errors. If the files throwing errors also vary, then it's probably caused by a random manipulation of the formatting of the input file. As of 2019_10_29, the only place in the application where there is a random number generator is when I decide whether a waveform is flipped upside down or not.*

## 5. Detailed description of the application's procedure

It's real simple:

1. `main()` **start** **:**
    1. **initialize RNG**
    1. **import all files from** `resources/input`
        * `read_directory()`
    1. **if *hasCat* or *nonCat* it's largely the same procedure** **:**
        1. **prep output file(s)**
            * `getCategorizationFileNames()` if ***hasCat***
            * `openCategorizationFiles()` if ***hasCat***
        1. **write the *first row of the file*, made up of labels** **:**
            1. ***get the*** *`(label,repetition)`* ***pairs***
                * `getFeatureHeader()`
                    * `getFeatureNamesAbbrev_hasCat()`
                    * `getFeatureRepetitions_hasCat()`
                    * `getFeatureNamesAbbrev_nonCat()`
                    * `getFeatureRepetitions_nonCat()`
            1. ***convert those to a base row vector with just labels***
                * `getFileHeaderBase()`
            1. ***write it to the file. If*** *`nonCat`*, ***then this step also serves to trim the fat out of the base row vector.***
                * `writeFileHeaderSans()` if ***nonCat***
                * `writeAllHeadersTree()` if ***hasCat***
                    * `writeFileHeaderTree()`
        1. `for` **each of the input files** **:**
            1. ***read the damn thing***
                * `readANTSfile()`
            1. ***sum the channels to form the signal***
                * `sumChannels()`
                    * `a_column2a_signal()`
                        * `taperOutEmptyElements()`
            1. ***randomly choose if the signal is upside down or not***
                * `signalUpsideDown()`
            1. ***get the raw parameters***
                * `getParams()`
                    * *(see below all the things this calls)*
            1. ***get the features from the parameters***
                * `getFeatures()`
            1. ***write down the feature values to the (correct) output file***
                * `writeLineToFile()` with parameter overloading for ***Cat*** cases
                    * `a_Features2a_sentence_hasCat()`
                    * `a_Features2a_sentence_nonCat()`
        1. **end of** `for`
        1. **close the output file(s)**
            * `closeCategorizationFiles()` if ***hasCat***
1. **end of** `main()`

Here are all the things the `getParams()` calls:

* `getParams()`
    1. `signalSansSeparatedBremsstrahlung()`
        * `signalAbsSum()`
    1. `getID()`
        * `splitWP()`
    1. `getSignalCumAreas()`
    1. `getSignalLength()`
    1. `getPeakHeightsAndTime()`
        * `smooth()`
            * `dumbSmooth()`
        * `dumbDerivative()`
        * `getZeroIndices()`
            * `fracIdx()`
            * `rFracIdx()`
            * `sameSign()`
        * `signalFlipFlopper()`
            * `cat()` (WARNING: this cat means "concatenate". Confusing, I know.)
    1. `getHeightDistOfPeaks()`
    1. `getTimeDistOfPeaks()`
    1. `getMaxPk_HT()`
    1. `getAfts()`
        * `nanosec2idx()`
    1. `getRMSwidth()`
    1. `getSignalMean()`
    1. `getFWHM()`
    1. `getRMSamplitude()`
    1. `getPromptFraction50ns()`
        * `t2idx()`
            * `nanosec2idx()`
    1. `getFixedWindowAreas()`
        * `t2idx()`
    1. `getParamNames()`

## 6. Detailed description of what each principal function does

### 6.1. *myTypedefs.h*

Not a function, but it's worth mentioning it anyway. It used to be that  every file had its own typedefs, which generated conflicts when there were several typedefs with the same name throughout the application.  The way I found of fixing that is placing all the application's typedefs in a single file that gets inherited by everything. Now, if you look at the source code, you may notice that I use typedefs for *almost everything*. Hell, `myTypedefs.h` even has three different typedefs back to back just for doubles. What the hell? Well, the idea with these is to make it easier to keep track of what thing in the application serves what purpose. So for example those three typedefs represent different units of measurement. That way it's easier to tell when there's an apples and oranges type of situation going on, and hopefully it keeps easily preventable mistakes from being made. Also it means that if I eventually decide that "no, this thing should be *this* type instead", I can change one line of code in `myTypedefs.h` and never have to worry about it again.

### 6.2. consoleMessage_WP(), ..._PF() and ..._main() from *consoleMessages*

These functions serve to write things to the terminal. The reason why I put them here and not, say, in the code itself is basically to prevent clutter. I want the code to be easily readable, and having every block of important stuff be separated by a helpful phrase would just space the whole thing out to the point of unweildiness. If you want to play around with these functions, fine, go ahead, but it's nothing of interest, to be honest. `consoleMessages.h` is inherited by `waveformParametrizer.h`, `parameterFeaturizer.h` and `main.cpp`, because those three are the files with the most "involved" procedures, it's the ones where it's most important to keep track of what is happening.

### 6.3. read_directory() from *getFilesInDir*

`read_directory()` wants nothing more but to find every filename inside a selected directory and return it in a `std::vector` of `string`s. It uses this `<experimental/filesystem>` package from C++17, and I still can't believe I actually got it working. Please no touchy here.

### 6.4. readANTSfile() from *readANTSoutput*

It reads a waveform file regurgitated by ANTS2 and converts it into an `a_cell` thing salad, so it outputs an `std::map` where the signal from each PMT is labeled by the corresponding PMT name, however at this stage the signals themselves are just a series of strings where the numbers correspond to the amplitudes. Preferably you shouldn't touch this function: the formatting of those ANTS outputs is really finnicky.

### 6.5. saveCellToFile() from *readANTSoutput*

Uhhh, I think this one is a leftover from an earlier application. It's not used in the paraturizer. In case you're wondering, it serves to convert the `std::a_cell` to a table in a file, and presumably the idea back then would have been to then read the file in MATLAB and output the waveform that way. In the meantime that was all scrapped, but this little fossil remains. Oh well. You don't really need it.

### 6.6. sumChannels() from *readANTSoutput*

It returns the sum of every PMT signal. That's all you need to know. Don't look at it please, I embarris

### 6.7. signalUpsideDown() from *readANTSoutput*

It reverses an `a_signal` so that it's as if the deposition happened upside down in relation to its original orientation. It also does some zero padding, because otherwise the application kept crashing for some reason.

### 6.8. getParams() from *waveformParametrizer*

Populates an `a_Params` with parametrization info on the waveform, namely the area, numbers of peaks, all that good stuff. It's basically the most important function in the program: it mines your lousy waveforms for valuable raw information. Browse through it at your leisure, add stuff to it, you name it.

### 6.9 getFeatureHeader() from *parameterFeaturizer*

Because the feature extraction tables are constructed line-by-line, it is important to know what the headers look like a priori. Hence why this is a separate function from `getFeatures()`. The header is written to the files before anything else, and this part is in charge of knowing what the names of the features are, as well as the number of times each feature can show up (so for example for a two peak event, you'd expect the peak time to show up twice). The function outputs a so-called `featHed`. That is a vector of name-number pairs, representing the names of the features and the number of "spaces" each feature occupies. So for example if you have two peaks, you will have a `featHed` entry that looks something like **("pk_ns", 2)**.

### 6.10 getFeatures() from *parameterFeaturizer*

It picks up the `a_Params` structure given by `getParams()` and it manipulates the stuff there to turn it into the features that then go into a file, and outputs them as a so-called `a_Features` structure.

### 6.11 getFileHeaderBase() from *fileOutput*

It returns a base version of the first row of the outputted feature tables. Then this base version needs to be manipulated into the proper shape for the corresponding category. If there is no categorization, it still serves to convert the `featHed` into an `a_sentence`, that is a vector of strings, where each string is the name of one of the colums in the table. `main()` then has the function that serves to trim the `fileHeaderBase` into the correct shape and output it into the file itself.

### 6.12 a_Features2a_sentence_hasCat() and ..._nonCat() from *fileOutput*

Outputs the data from the `a_Feature` into the output table. You will have to fiddle with it when adding features, be careful not to mess with the internal structure, cause that would spell the end to the thing.

### 6.13 vectorGatherMaxElements() from *fileOutput*

Do you know what this function is for? Cause I sure don't. Haha.

