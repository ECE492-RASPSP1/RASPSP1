# RASPSP1
Capstone Project - Robot Arm Satellite Positioning Simulator

# Contribution guide

## Branching
<ol>
    <li>
    To create a branch, you will first need to clone the repository to a directory on your machine. Open a terminal in the directory you wish to place the  repository files and then type <code>git clone &lt;https link to repo&gt; </code> <br>
    Example: <code>git commit https://github.com/dany-cpp/QuantumPermutationPad.git </code>
    </li>
    <li>
    After you have cloned the repository, change your terminal directory to the repository you just cloned and type <code>git checkout -b &lt;name of your branch&gt; </code> <br>
    Example: <code>git checkout -b MyName-new-branch </code> <br>
    The checkout command switches your working branch, and the -b flag creates a new branch.
    </li>
    <li>
    Branch, name, if linked to an issue, must contains the issue number in this format. For example "MP-10" for PR linked to issue #10. 
    </li>
    <li>
    You can check all of the local branches on your machines using  <code>git branch </code> <br>
    If another member has created new remote branches that are not yet on your machine, you can access those branches using <br> <code> git fetch </code>
    </li>
</ol>


## Pull request
<ol>
    <li>
    First, stage your changes by using <code>git add &lt;changed_file&gt; </code> <br>
    Example: <code>git add /C++/my_code.cpp </code> <br>
    Do not use <code>git add .</code> because it will stage unnecessary files. A goode IDE can help you with this.
    </li>
    <li>
    Commit by using <code>git commit -m "My message"</code>. Give it a meaningful one such as "Fixed bug for input" instead of "Minor fix"
    </li>
    <li>
    Rebasing from main meaning you update your code. This is due to because during you work on your branch, other people
    have made updates. To do this, use <code>git rebase -i origin/main</code>. After that, type <code>:wq</code> and Enter.
    This is Vim syntax, ask me for more details about this step.
    </li>
    <li>
    Push your changes via <code>git push</code>.
    </li>
    <li>
    On the project page [Here]. Click on Pull Request tab. Click on New Pull Request
    button, and create one from your branch to main.
    </li>
    <li>
    Ping on Discord to get it reviewed.
    </li>
</ol>

___

# Compile and execute guide

## For C++ code
<ol>
    <li>
    In the C++ directory, you will find the Makefile. In line 38, change it to <code>MODE := $(RELEASE)</code> or <code>MODE := $(DEBUG)</code> as you wish.
    Currently, we only support these 2 modes. Note that to use GDB to debug, you must use Debug profile.
    </li>
    <li>
    In the same directory use <code>make</code> the compile the code; <code>make clean</code> to clean everything.
    </li>
    <li>
    If the process run successfully, you will find a new binary in <code>bin/</code>. To execute, run <code>./bin/qpp.exe</code>.
    </li>
</ol>
