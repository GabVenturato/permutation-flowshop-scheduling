# Permutation FlowShop Scheduling
Project developed in team with Alessio Chiapperini.

## Project Structure
Developed solutions using different techniques:
- MiniZinc model
- Enumeration
- Greedy
- Local Search

## Greedy
In greedy our aim was to minimize the tardiness since the instances we have are
higly influenced by it. We developed a naive and not very meaningful idea with 
good results in smaller instances, and a more advanced idea that works better 
with bigger instances. By the way both of them are not such good. An 
improvement can be probably performed dividing the cost components, and ordering 
elements in more than one way. Further ideas need to be deeped.

## Local Search
In Local Search we used the library EasyLocal (commit of Tue Dec 11), references
can be found in its subfolder.
We developed two moves: SwapJobs and MoveJob. Further details can be found in 
the code. An improvement can be maybe reached modifing the "MoveJob" move such 
that it lets move jobs forward and backward (now it lets to move only forward).
It's important to take care to the case `(i,i+1)` because it produces the side
effect of moving twice on the same neighbor.
