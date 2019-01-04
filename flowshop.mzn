include "alldifferent.mzn";

int: Jobs;
int: Machines;

array[1..Jobs, 1..Machines] of int: ProcessingTimes;
array[1..Jobs] of int: ReleaseDates;
array[1..Jobs] of int: DueDates;
array[1..Jobs] of int: Weights;

int: MaxExecTime = max(ReleaseDates) + sum (j in 1..Jobs, i  in 1..Machines) (ProcessingTimes[j,i]); % NOT SURE

% VARIABLES
array[1..Jobs, 1..Machines] of var 0..MaxExecTime: Execution;
array[1..Jobs] of var 1..Jobs: JobsOrder;

var int: Cost;

% CONSTRAINTS
constraint alldifferent(JobsOrder);

% first job should start as soon as possible
constraint
   Execution[ JobsOrder[1], 1 ] = min(ReleaseDates);

% i-th operation must start after (i-1)-th operation
constraint
    forall (j in 1..Jobs, m in 2..Machines) (
        Execution[JobsOrder[j],m] >= Execution[JobsOrder[j],m-1] + ProcessingTimes[JobsOrder[j],m-1]
    );

% every machine can run one job per time
constraint
    forall (j in 2..Jobs, m in 1..Machines) (
        Execution[JobsOrder[j], m] >= Execution[JobsOrder[j-1], m] + ProcessingTimes[JobsOrder[j-1],m]
    );

% a job can't start before its release date
constraint
    forall (j in 1..Jobs) (
        Execution[JobsOrder[j],1] >= ReleaseDates[JobsOrder[j]]
    );

% OBJECTIVE FUNCTION
constraint 
    Cost = max([Execution[JobsOrder[j],Machines] + ProcessingTimes[JobsOrder[j],Machines] | j in 1..Jobs]) + 
        sum ( j in 1..Jobs ) (
            let { var int: Diff = 
                    if (DueDates[JobsOrder[j]] == -1) 
                    then 0
                    else (Execution[JobsOrder[j],Machines] + ProcessingTimes[JobsOrder[j],Machines]) - DueDates[JobsOrder[j]]
                    endif } in
            if ( Diff <= 0 ) 
            then 0
            else Diff * Weights[JobsOrder[j]]
            endif
        );


solve minimize Cost;

% OUTPUT
output
    ["EXECUTION TIMES:\n"] ++
    [if ( m == Machines ) 
        then show_int(5,Execution[j,m]) ++ "(" ++ show_int(5,Execution[j,m]+ProcessingTimes[j,m]) ++ ")\n"
        else show_int(5,Execution[j,m]) ++ "(" ++ show_int(5,Execution[j,m]+ProcessingTimes[j,m]) ++ ")\t"
    endif | j in 1..Jobs, m in 1..Machines ]
    ++ [ "\n\nJOBS ORDER:\t" ++ show(JobsOrder) ]
    ++ [ "\n\nCOST:\t" ++ show(Cost) ]