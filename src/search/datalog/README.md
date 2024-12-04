The original Datalog code here was used to compute delete-relaxed heuristics, as
in Correa et al., 2021 and 2022. This fragment of Datalog has no negatives in
rule bodies. The DatalogLiteral class was added as a placeholder for handling
axioms (stratified-negated Datalog), where rule bodies now contain literals
(positive or negative atoms). However, none of the underyling Datalog code has
been extended to handle stratified-negated Datalog, and only changed so that the
previous code compiles and works.
