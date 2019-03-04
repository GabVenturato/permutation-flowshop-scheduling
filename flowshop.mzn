include "alldifferent.mzn";

int: Jobs;
int: Machines;

array[1..Jobs, 1..Machines] of int: ProcessingTimes;
array[1..Jobs] of int: ReleaseDates;
array[1..Jobs] of int: DueDates;
array[1..Jobs] of int: Weights;

% To limit Execution time domain:
% - at least 0 value is needed to start
% - max value can be the sum of all jobs executed one per time, plus max release time
int: MaxExecTime = max(ReleaseDates) + sum (j in 1..Jobs, i  in 1..Machines) (ProcessingTimes[j,i]);

% VARIABLES
array[1..Jobs, 1..Machines] of var 0..MaxExecTime: Execution;
array[1..Jobs] of var 1..Jobs: JobsOrder;

var int: Cost;

% CONSTRAINTS
constraint alldifferent(JobsOrder);

% i-th operation must start after (i-1)-th operation
constraint
    forall (j in 1..Jobs, m in 2..Machines) (
        Execution[j,m] >= Execution[j,m-1] + ProcessingTimes[j,m-1]
    );

% every machine can run one job per time
constraint
    forall (j in 2..Jobs, m in 1..Machines) (
        Execution[JobsOrder[j], m] >= Execution[JobsOrder[j-1], m] + ProcessingTimes[JobsOrder[j-1],m]
    );

% a job can't start before its release date
constraint
    forall (j in 1..Jobs) (
        Execution[j,1] >= ReleaseDates[j]
    );

% first job executed ends without delays between operations
constraint
    Execution[JobsOrder[1],Machines] = ReleaseDates[JobsOrder[1]] +
        sum (m in 1..Machines-1) ( ProcessingTimes[JobsOrder[1],m] );

% OBJECTIVE FUNCTION
constraint
    Cost = max([Execution[j,Machines] + ProcessingTimes[j,Machines] | j in 1..Jobs]) +
        sum ( j in 1..Jobs where DueDates[j] != -1 ) (
            let { 
                var int: Diff = (Execution[j,Machines] + ProcessingTimes[j,Machines]) - DueDates[j] 
            } in bool2int(Diff>0) * Diff * Weights[j]
        );

% SOLVE
% solve minimize Cost;

% solve :: int_search( [ Execution[j,m] | j in 1..Jobs, m in 1..Machines], first_fail, indomain_min, complete )
%     minimize Cost;

solve :: int_search( JobsOrder, first_fail, indomain_median, complete )
    minimize Cost;

% OUTPUT
output
    ["EXECUTION TIMES:\n"] ++
    [if ( m == Machines )
        then show_int(5,Execution[j,m]) ++ "(" ++ show_int(5,Execution[j,m]+ProcessingTimes[j,m]) ++ ")\n"
        else show_int(5,Execution[j,m]) ++ "(" ++ show_int(5,Execution[j,m]+ProcessingTimes[j,m]) ++ ")\t"
    endif | j in 1..Jobs, m in 1..Machines ]
    ++ [ "\n\nJOBS ORDER:\t" ++ show(JobsOrder) ]
    ++ [ "\n\nCOST:\t" ++ show(Cost) ]
