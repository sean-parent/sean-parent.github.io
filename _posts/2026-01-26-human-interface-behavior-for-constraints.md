---
layout: post
title: Human Interface Behavior for Constraints
description: ''
date: 2026-01-26
published: false
---

## Introduction

This paper explores different behavioral patterns for reflecting constraints in human interfaces. Constraints are an unavoidable aspect of the underlying data and command models of software applications. These constraints are what gives software meaning. To illustrate, simple addition has no meaning if the result could be _anything_. What makes addition _addition_ is the _constraint_ that the result must be the sum of the arguments. Further, allowing the user to set artifical constraints can assist them in achieving a desired goal. For example, locking the aspect ratio when cropping an image is a useful behavior even when the underlying data and command model do not impose such a constraint.

Much of existing UI design is concerned with appearence. Behavior, and specifically general rules regarding behavior, are often left as an artifact of the implementation leading to bug reports regarding consistency and _expected_ behavior.

The lack of consistency means users are not able to accurately predict the result of an action, achieving a desired outcome may be more challenging than necessary, and the interface may impose unnessary restrictions on the states that can be achieved.

This paper will show that the challenge is one of balancing a set of often conflicting goals to achieve the best, most intuitive, result. This paper can serve as a framework for user studies to explore the space and codify a set of rules that can be applied to all human interface implementations to improve both correctness and consistency.

## Methodology

This paper presents a series of common constrains and a collection of HI demonstrations for each. The constraints chosen are done so to illustrate different choices and potential rules. The list is not exhastive, and the set of possible presentations is not exhaustive. Constraints may interconnect which futher complicates the application of any rules.

At scale, the approach should be to create a set of rules, and when an interface instance is encountered where another set of choices yields a better outcome, to extract from that a refined set of rules. Ideally these rules should be abstract and generally applicable, but human interfaces are language and any language evolves with a set of exceptions established by historical convention. Human interfaces cannot be fully evaluated in issolation but must be evaluated in the context of existing conventions, including platform and locale specific conventions.

Any user study should start with a survey of existing solutions.

## Demo 1: Simple Implication

In this implementation, when `a` is selected, `b` is automatically selected and disabled. When `a` is deselected, `b` remains selected but becomes enabled. This approach has no memory of the user's previous choice for `b`.

<div style="padding: 20px; border: 2px solid #ccc; border-radius: 8px; margin: 20px 0; background: #f9f9f9;">
  <h4 style="margin-top: 0;">Demo 1: No Memory</h4>
  <label style="display: block; margin: 10px 0;">
    <input type="checkbox" id="demo1_a" style="margin-right: 8px;">
    <strong>Option A</strong> (constraint source)
  </label>
  <label style="display: block; margin: 10px 0;">
    <input type="checkbox" id="demo1_b" style="margin-right: 8px;">
    <strong>Option B</strong> (constrained by A)
  </label>
</div>

<script>
(function() {
  const a = document.getElementById('demo1_a');
  const b = document.getElementById('demo1_b');
  const status = document.getElementById('demo1_status');
  
  function updateStatus() {
    status.textContent = `A: ${a.checked}, B: ${b.checked}, B disabled: ${b.disabled}`;
  }
  
  a.addEventListener('change', function() {
    if (a.checked) {
      b.checked = true;
      b.disabled = true;
    } else {
      b.disabled = false;
      // b remains checked
    }
    updateStatus();
  });
  
  b.addEventListener('change', updateStatus);
  updateStatus();
})();
</script>

**Behavior**: Check A → B becomes checked and disabled. Uncheck A → B remains checked but is now enabled.

---

## Demo 2: Implication with Memory

This implementation remembers the user's previous choice for `b`. When `a` is selected, `b` appears disabled but its prior state is preserved. When `a` is deselected, `b` returns to its previously set state.

<div style="padding: 20px; border: 2px solid #ccc; border-radius: 8px; margin: 20px 0; background: #f9f9f9;">
  <h4 style="margin-top: 0;">Demo 2: With Memory</h4>
  <label style="display: block; margin: 10px 0;">
    <input type="checkbox" id="demo2_a" style="margin-right: 8px;">
    <strong>Option A</strong> (constraint source)
  </label>
  <label style="display: block; margin: 10px 0;">
    <input type="checkbox" id="demo2_b" style="margin-right: 8px;">
    <strong>Option B</strong> (constrained by A, with memory)
  </label>
</div>

<script>
(function() {
  const a = document.getElementById('demo2_a');
  const b = document.getElementById('demo2_b');
  const status = document.getElementById('demo2_status');
  let rememberedB = false;
  
  function updateStatus() {
    status.textContent = `A: ${a.checked}, B: ${b.checked}, B disabled: ${b.disabled}, Remembered B: ${rememberedB}`;
  }
  
  a.addEventListener('change', function() {
    if (a.checked) {
      rememberedB = b.checked; // Save current state
      b.checked = true;
      b.disabled = true;
    } else {
      b.disabled = false;
      b.checked = rememberedB; // Restore saved state
    }
    updateStatus();
  });
  
  b.addEventListener('change', function() {
    if (!b.disabled) {
      rememberedB = b.checked;
    }
    updateStatus();
  });
  
  updateStatus();
})();
</script>

**Behavior**: Check A → B becomes checked and disabled (prior state saved). Uncheck A → B returns to its saved state.

---

## Demo 3: Contrapositive (!b ==> !a)

Instead of disabling `b`, this implementation enforces the logical contrapositive: if `b` is false, then `a` must be false. When `a` is checked, `b` is automatically checked. If you then uncheck `b`, `a` is automatically unchecked as well.

<div style="padding: 20px; border: 2px solid #ccc; border-radius: 8px; margin: 20px 0; background: #f9f9f9;">
  <h4 style="margin-top: 0;">Demo 3: Contrapositive (No Memory)</h4>
  <label style="display: block; margin: 10px 0;">
    <input type="checkbox" id="demo3_a" style="margin-right: 8px;">
    <strong>Option A</strong> (constraint source)
  </label>
  <label style="display: block; margin: 10px 0;">
    <input type="checkbox" id="demo3_b" style="margin-right: 8px;">
    <strong>Option B</strong> (contrapositive constraint)
  </label>
</div>

<script>
(function() {
  const a = document.getElementById('demo3_a');
  const b = document.getElementById('demo3_b');
  const status = document.getElementById('demo3_status');
  
  function updateStatus() {
    status.textContent = `A: ${a.checked}, B: ${b.checked}`;
  }
  
  a.addEventListener('change', function() {
    if (a.checked) {
      b.checked = true; // a ==> b
    }
    updateStatus();
  });
  
  b.addEventListener('change', function() {
    if (!b.checked) {
      a.checked = false; // !b ==> !a (contrapositive)
    }
    updateStatus();
  });
  
  updateStatus();
})();
</script>

**Behavior**: Check A → B is checked. Uncheck B → A is automatically unchecked (contrapositive enforcement).

---

## Demo 4: Contrapositive with Memory

This combines the contrapositive logic with memory. The system remembers user-set states and restores them when the constraint is no longer forcing a value.

<div style="padding: 20px; border: 2px solid #ccc; border-radius: 8px; margin: 20px 0; background: #f9f9f9;">
  <h4 style="margin-top: 0;">Demo 4: Contrapositive with Memory</h4>
  <label style="display: block; margin: 10px 0;">
    <input type="checkbox" id="demo4_a" style="margin-right: 8px;">
    <strong>Option A</strong> (constraint source)
  </label>
  <label style="display: block; margin: 10px 0;">
    <input type="checkbox" id="demo4_b" style="margin-right: 8px;">
    <strong>Option B</strong> (contrapositive with memory)
  </label>
</div>

<script>
(function() {
  const a = document.getElementById('demo4_a');
  const b = document.getElementById('demo4_b');
  const status = document.getElementById('demo4_status');
  let rememberedA = false;
  let rememberedB = false;
  let aIsConstrained = false;
  let bIsConstrained = false;
  
  function updateStatus() {
    const aState = aIsConstrained ? 'constrained' : 'user-set';
    const bState = bIsConstrained ? 'constrained' : 'user-set';
    status.textContent = `A: ${a.checked} (${aState}, mem: ${rememberedA}), B: ${b.checked} (${bState}, mem: ${rememberedB})`;
  }
  
  a.addEventListener('change', function() {
    if (!aIsConstrained) {
      rememberedA = a.checked;
    }
    
    if (a.checked) {
      // a ==> b: constrain b to true
      if (!bIsConstrained) {
        rememberedB = b.checked;
      }
      b.checked = true;
      bIsConstrained = true;
      aIsConstrained = false;
    } else {
      // a is false, b is no longer constrained
      bIsConstrained = false;
      b.checked = rememberedB;
      aIsConstrained = false;
    }
    updateStatus();
  });
  
  b.addEventListener('change', function() {
    if (!bIsConstrained) {
      rememberedB = b.checked;
    }
    
    if (!b.checked) {
      // !b ==> !a: constrain a to false
      if (!aIsConstrained) {
        rememberedA = a.checked;
      }
      a.checked = false;
      aIsConstrained = true;
      bIsConstrained = false;
    } else {
      // b is true, a is no longer constrained by contrapositive
      if (aIsConstrained) {
        aIsConstrained = false;
        a.checked = rememberedA;
      }
      bIsConstrained = false;
    }
    updateStatus();
  });
  
  updateStatus();
})();
</script>

**Behavior**: Check A → B is constrained to checked. Uncheck B → A is constrained to unchecked. When constraints are released, values return to their last user-set state. Try this sequence: Check A, Check B, Uncheck A, Uncheck B (notice A returns to its remembered state).

---

## Analysis

These four demos illustrate fundamentally different approaches to handling constraints in user interfaces:

1. **No Memory, Disabled**: Simple but loses user intent
2. **Memory, Disabled**: Preserves intent but hides the actual state while constrained
3. **Contrapositive, No Memory**: Maintains logical consistency through active enforcement
4. **Contrapositive, Memory**: Combines logical enforcement with preservation of user intent

Each approach has different implications for user understanding, predictability, and the preservation of user choices.
