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

% first job executed ends without delays between operations
constraint
    Execution[JobsOrder[1],Machines] = ReleaseDates[JobsOrder[1]] + sum (m in 1..Machines-1) ( ProcessingTimes[JobsOrder[1],m] );

% i-th operation must start after (i-1)-th operation
constraint
    sum (j in 1..Jobs, m in 2..Machines) (
        bool2int ( Execution[JobsOrder[j],m] < Execution[JobsOrder[j],m-1] + ProcessingTimes[JobsOrder[j],m-1] )
    ) = 0;

constraint
    forall (j in 1..Jobs-1, m in 1..Machines) (
        Execution[JobsOrder[j],m] < Execution[JobsOrder[j+1],m]
    );

constraint
    forall (j in 1..Jobs, m in 1..Machines-1) (
        Execution[JobsOrder[j],m] < Execution[JobsOrder[j],m+1]
    );

% every machine can run one job per time
constraint
    sum (j in 2..Jobs, m in 1..Machines) (
        bool2int ( Execution[JobsOrder[j], m] < Execution[JobsOrder[j-1], m] + ProcessingTimes[JobsOrder[j-1],m] )
    ) = 0;

% a job can't start before its release date
constraint
    sum (j in 1..Jobs) (
        bool2int ( Execution[JobsOrder[j],1] < ReleaseDates[JobsOrder[j]] )
    ) = 0;

% OBJECTIVE FUNCTION
% int: MaxTardiness = sum (j in 1..Jobs) (
%     let { int: d = sum(m in 1..Machines) (ProcessingTimes[j,m]) - DueDates[j] } in
%         if (d <= 0)
%         then 0
%         else d * Weights[j]
%         endif
% );

int: MaxDiff = max (j in 1..Jobs) (
    ReleaseDates[j] + sum(m in 1..Machines) (ProcessingTimes[j,m]) - DueDates[j]
);

var int: Makespan;
var int: Tardiness;
array[1..Jobs] of var 1..MaxDiff: Diff;

constraint Makespan = max([Execution[JobsOrder[j],Machines] + ProcessingTimes[JobsOrder[j],Machines] | j in 1..Jobs]);

constraint
    forall (j in 1..Jobs) (
        Diff[JobsOrder[j]] = 
            bool2int(DueDates[JobsOrder[j]] != -1) * (Execution[JobsOrder[j],Machines] + ProcessingTimes[JobsOrder[j],Machines]) - DueDates[JobsOrder[j]]
    );

constraint Tardiness = sum (j in 1..Jobs) ( bool2int(Diff[j]>0) * abs(Diff[j]) * Weights[JobsOrder[j]] );

constraint Cost = Makespan + Tardiness;


% solve minimize Cost;
solve :: int_search( [ Execution[j,m] | j in 1..Jobs, m in 1..Machines], first_fail, indomain_min, complete )
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