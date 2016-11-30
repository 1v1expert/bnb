
//          Copyright Joakim Karlsson & Kim Gräsman 2010-2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IGLOO_FLUENT_H
#define IGLOO_FLUENT_H

#include "constraintlist.h"
#include "constraintadapter.h"
#include "bandit/assertion_frameworks/snowhouse/snowhouse/fluent/operators/constraintoperator.h"
#include "bandit/assertion_frameworks/snowhouse/snowhouse/fluent/operators/andoperator.h"
#include "bandit/assertion_frameworks/snowhouse/snowhouse/fluent/operators/oroperator.h"
#include "bandit/assertion_frameworks/snowhouse/snowhouse/fluent/operators/collections/collectionconstraintevaluator.h"
#include "bandit/assertion_frameworks/snowhouse/snowhouse/fluent/operators/collections/alloperator.h"
#include "bandit/assertion_frameworks/snowhouse/snowhouse/fluent/operators/collections/noneoperator.h"
#include "bandit/assertion_frameworks/snowhouse/snowhouse/fluent/operators/collections/atleastoperator.h"
#include "bandit/assertion_frameworks/snowhouse/snowhouse/fluent/operators/collections/exactlyoperator.h"
#include "bandit/assertion_frameworks/snowhouse/snowhouse/fluent/operators/collections/atmostoperator.h"
#include "bandit/assertion_frameworks/snowhouse/snowhouse/fluent/operators/notoperator.h"
#include "expressionbuilder.h"

namespace snowhouse {

  inline ExpressionBuilder<Nil> Is()
  {
    return ExpressionBuilder<Nil>(Nil());
  }

  inline ExpressionBuilder<Nil> Has()
  {
     return ExpressionBuilder<Nil>(Nil());
  }

}

#endif
