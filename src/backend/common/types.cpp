//===----------------------------------------------------------------------===//
//
//							PelotonDB
//
// types.cpp
//
// Identification: src/backend/common/types.cpp
//
// Copyright (c) 2015, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//


#include "backend/common/types.h"
#include "backend/common/exception.h"

#include <sstream>
#include <cstring>

namespace peloton {

ItemPointer INVALID_ITEMPOINTER;

//===--------------------------------------------------------------------===//
// Type utilities
//===--------------------------------------------------------------------===//



/// Works only for fixed-length types
std::size_t GetTypeSize(ValueType type) {
  switch (type) {
    case (VALUE_TYPE_TINYINT):
      return 1;
    case (VALUE_TYPE_SMALLINT):
      return 2;
    case (VALUE_TYPE_INTEGER):
      return 4;
    case (VALUE_TYPE_BIGINT):
      return 8;
    case (VALUE_TYPE_DOUBLE):
      return 8;
    case (VALUE_TYPE_VARCHAR):
      return 0;
    case (VALUE_TYPE_VARBINARY):
      return 0;
    case (VALUE_TYPE_TIMESTAMP):
      return 8;
    case (VALUE_TYPE_DECIMAL):
      return 0;
    case (VALUE_TYPE_INVALID):
      return 0;
    case (VALUE_TYPE_NULL):
      return 0;
    default: { return 0; }
  }
}

//===--------------------------------------------------------------------===//
// BackendType <--> String Utilities
//===--------------------------------------------------------------------===//
std::string BackendTypeToString(BackendType type) {
  std::string ret;

  switch (type) {
    case (BACKEND_TYPE_VM):
      return "VOLATILE";
    case (BACKEND_TYPE_NVM):
      return "NON-VOLATILE";
    case (BACKEND_TYPE_INVALID):
      return "INVALID";
    default: {
      char buffer[32];
      ::snprintf(buffer, 32, "UNKNOWN[%d] ", type);
      ret = buffer;
    }
  }
  return (ret);
}

BackendType StringToBackendType(std::string str) {
  if (str == "INVALID") {
    return BACKEND_TYPE_INVALID;
  } else if (str == "VOLATILE") {
    return BACKEND_TYPE_VM;
  } else if (str == "NON-VOLATILE") {
    return BACKEND_TYPE_NVM;
  }
  return BACKEND_TYPE_INVALID;
}

//===--------------------------------------------------------------------===//
// Value <--> String Utilities
//===--------------------------------------------------------------------===//

std::string ValueTypeToString(ValueType type) {
  switch (type) {
    case VALUE_TYPE_INVALID:
      return "INVALID";
    case VALUE_TYPE_NULL:
      return "NULL";
    case VALUE_TYPE_TINYINT:
      return "TINYINT";
    case VALUE_TYPE_SMALLINT:
      return "SMALLINT";
    case VALUE_TYPE_INTEGER:
      return "INTEGER";
    case VALUE_TYPE_BIGINT:
      return "BIGINT";
    case VALUE_TYPE_DOUBLE:
      return "FLOAT";
    case VALUE_TYPE_VARCHAR:
      return "VARCHAR";
    case VALUE_TYPE_VARBINARY:
      return "VARBINARY";
    case VALUE_TYPE_TIMESTAMP:
      return "TIMESTAMP";
    case VALUE_TYPE_DECIMAL:
      return "DECIMAL";
    default:
      return "INVALID";
  }
}

ValueType StringToValueType(std::string str) {
  if (str == "INVALID") {
    return VALUE_TYPE_INVALID;
  } else if (str == "NULL") {
    return VALUE_TYPE_NULL;
  } else if (str == "TINYINT") {
    return VALUE_TYPE_TINYINT;
  } else if (str == "SMALLINT") {
    return VALUE_TYPE_SMALLINT;
  } else if (str == "INTEGER") {
    return VALUE_TYPE_INTEGER;
  } else if (str == "BIGINT") {
    return VALUE_TYPE_BIGINT;
  } else if (str == "FLOAT") {
    return VALUE_TYPE_DOUBLE;
  } else if (str == "STRING") {
    return VALUE_TYPE_VARCHAR;
  } else if (str == "VARBINARY") {
    return VALUE_TYPE_VARBINARY;
  } else if (str == "TIMESTAMP") {
    return VALUE_TYPE_TIMESTAMP;
  } else if (str == "DECIMAL") {
    return VALUE_TYPE_DECIMAL;
  } else {
    throw ConversionException("No conversion from string :" + str);
  }
  return VALUE_TYPE_INVALID;
}

/** takes in 0-F, returns 0-15 */
int32_t HexCharToInt(char c) {
  c = static_cast<char>(toupper(c));
  if ((c < '0' || c > '9') && (c < 'A' || c > 'F')) {
    return -1;
  }
  int32_t retval;
  if (c >= 'A')
    retval = c - 'A' + 10;
  else
    retval = c - '0';

  assert(retval >= 0 && retval < 16);
  return retval;
}

bool HexDecodeToBinary(unsigned char *bufferdst, const char *hexString) {
  assert(hexString);
  size_t len = strlen(hexString);
  if ((len % 2) != 0) return false;
  uint32_t i;
  for (i = 0; i < len / 2; i++) {
    int32_t high = HexCharToInt(hexString[i * 2]);
    int32_t low = HexCharToInt(hexString[i * 2 + 1]);
    if ((high == -1) || (low == -1)) return false;
    int32_t result = high * 16 + low;

    assert(result >= 0 && result < 256);
    bufferdst[i] = static_cast<unsigned char>(result);
  }
  return true;
}

//===--------------------------------------------------------------------===//
// Expression - String Utilities
//===--------------------------------------------------------------------===//

std::string ExpressionTypeToString(ExpressionType type) {
  switch (type) {
    case EXPRESSION_TYPE_INVALID: {
      return "INVALID";
    }
    case EXPRESSION_TYPE_OPERATOR_PLUS: {
      return "OPERATOR_PLUS";
    }
    case EXPRESSION_TYPE_OPERATOR_MINUS: {
      return "OPERATOR_MINUS";
    }
    case EXPRESSION_TYPE_OPERATOR_MULTIPLY: {
      return "OPERATOR_MULTIPLY";
    }
    case EXPRESSION_TYPE_OPERATOR_DIVIDE: {
      return "OPERATOR_DIVIDE";
    }
    case EXPRESSION_TYPE_OPERATOR_CONCAT: {
      return "OPERATOR_CONCAT";
    }
    case EXPRESSION_TYPE_OPERATOR_MOD: {
      return "OPERATOR_MOD";
    }
    case EXPRESSION_TYPE_OPERATOR_CAST: {
      return "OPERATOR_CAST";
    }
    case EXPRESSION_TYPE_OPERATOR_NOT: {
      return "OPERATOR_NOT";
    }
    case EXPRESSION_TYPE_OPERATOR_UNARY_MINUS: {
      return "OPERATOR_UNARY_MINUS";
    }
    case EXPRESSION_TYPE_COMPARE_EQ: {
      return "COMPARE_EQUAL";
    }
    case EXPRESSION_TYPE_COMPARE_NE: {
      return "COMPARE_NOT_EQUAL";
    }
    case EXPRESSION_TYPE_COMPARE_LT: {
      return "COMPARE_LESSTHAN";
    }
    case EXPRESSION_TYPE_COMPARE_GT: {
      return "COMPARE_GREATERTHAN";
    }
    case EXPRESSION_TYPE_COMPARE_LTE: {
      return "COMPARE_LESSTHANOREQUALTO";
    }
    case EXPRESSION_TYPE_COMPARE_GTE: {
      return "COMPARE_GREATERTHANOREQUALTO";
    }
    case EXPRESSION_TYPE_COMPARE_LIKE: {
      return "COMPARE_LIKE";
    }
    case EXPRESSION_TYPE_CONJUNCTION_AND: {
      return "CONJUNCTION_AND";
    }
    case EXPRESSION_TYPE_CONJUNCTION_OR: {
      return "CONJUNCTION_OR";
    }
    case EXPRESSION_TYPE_VALUE_CONSTANT: {
      return "VALUE_CONSTANT";
    }
    case EXPRESSION_TYPE_VALUE_PARAMETER: {
      return "VALUE_PARAMETER";
    }
    case EXPRESSION_TYPE_VALUE_TUPLE: {
      return "VALUE_TUPLE";
    }
    case EXPRESSION_TYPE_VALUE_TUPLE_ADDRESS: {
      return "VALUE_TUPLE_ADDRESS";
    }
    case EXPRESSION_TYPE_VALUE_NULL: {
      return "VALUE_NULL";
    }
    case EXPRESSION_TYPE_AGGREGATE_COUNT: {
      return "AGGREGATE_COUNT";
    }
    case EXPRESSION_TYPE_AGGREGATE_COUNT_STAR: {
      return "AGGREGATE_COUNT_STAR";
    }
    case EXPRESSION_TYPE_AGGREGATE_SUM: {
      return "AGGREGATE_SUM";
    }
    case EXPRESSION_TYPE_AGGREGATE_MIN: {
      return "AGGREGATE_MIN";
    }
    case EXPRESSION_TYPE_AGGREGATE_MAX: {
      return "AGGREGATE_MAX";
    }
    case EXPRESSION_TYPE_AGGREGATE_AVG: {
      return "AGGREGATE_AVG";
    }
    case EXPRESSION_TYPE_AGGREGATE_WEIGHTED_AVG: {
      return "AGGREGATE_WEIGHTED_AVG";
    }
    case EXPRESSION_TYPE_STAR: {
      return "STAR";
    }
    case EXPRESSION_TYPE_PLACEHOLDER: {
      return "PLACEHOLDER";
    }
    case EXPRESSION_TYPE_COLUMN_REF: {
      return "COLUMN_REF";
    }
    case EXPRESSION_TYPE_FUNCTION_REF: {
      return "FUNCTION_REF";
    }
    case EXPRESSION_TYPE_CAST: {
      return "CAST";
    }
  }
  return "INVALID";
}

ExpressionType StringToExpressionType(std::string str) {
  if (str == "INVALID") {
    return EXPRESSION_TYPE_INVALID;
  } else if (str == "OPERATOR_PLUS") {
    return EXPRESSION_TYPE_OPERATOR_PLUS;
  } else if (str == "OPERATOR_MINUS") {
    return EXPRESSION_TYPE_OPERATOR_MINUS;
  } else if (str == "OPERATOR_MULTIPLY") {
    return EXPRESSION_TYPE_OPERATOR_MULTIPLY;
  } else if (str == "OPERATOR_DIVIDE") {
    return EXPRESSION_TYPE_OPERATOR_DIVIDE;
  } else if (str == "OPERATOR_CONCAT") {
    return EXPRESSION_TYPE_OPERATOR_CONCAT;
  } else if (str == "OPERATOR_MOD") {
    return EXPRESSION_TYPE_OPERATOR_MOD;
  } else if (str == "OPERATOR_CAST") {
    return EXPRESSION_TYPE_OPERATOR_CAST;
  } else if (str == "OPERATOR_NOT") {
    return EXPRESSION_TYPE_OPERATOR_NOT;
  } else if (str == "COMPARE_EQUAL") {
    return EXPRESSION_TYPE_COMPARE_EQ;
  } else if (str == "COMPARE_NOTEQUAL") {
    return EXPRESSION_TYPE_COMPARE_NE;
  } else if (str == "COMPARE_LESSTHAN") {
    return EXPRESSION_TYPE_COMPARE_LT;
  } else if (str == "COMPARE_GREATERTHAN") {
    return EXPRESSION_TYPE_COMPARE_GT;
  } else if (str == "COMPARE_LESSTHANOREQUALTO") {
    return EXPRESSION_TYPE_COMPARE_LTE;
  } else if (str == "COMPARE_GREATERTHANOREQUALTO") {
    return EXPRESSION_TYPE_COMPARE_GTE;
  } else if (str == "COMPARE_LIKE") {
    return EXPRESSION_TYPE_COMPARE_LIKE;
  } else if (str == "CONJUNCTION_AND") {
    return EXPRESSION_TYPE_CONJUNCTION_AND;
  } else if (str == "CONJUNCTION_OR") {
    return EXPRESSION_TYPE_CONJUNCTION_OR;
  } else if (str == "VALUE_CONSTANT") {
    return EXPRESSION_TYPE_VALUE_CONSTANT;
  } else if (str == "VALUE_PARAMETER") {
    return EXPRESSION_TYPE_VALUE_PARAMETER;
  } else if (str == "VALUE_TUPLE") {
    return EXPRESSION_TYPE_VALUE_TUPLE;
  } else if (str == "VALUE_TUPLE_ADDRESS") {
    return EXPRESSION_TYPE_VALUE_TUPLE_ADDRESS;
  } else if (str == "VALUE_NULL") {
    return EXPRESSION_TYPE_VALUE_NULL;
  } else if (str == "AGGREGATE_COUNT") {
    return EXPRESSION_TYPE_AGGREGATE_COUNT;
  } else if (str == "AGGREGATE_COUNT_STAR") {
    return EXPRESSION_TYPE_AGGREGATE_COUNT_STAR;
  } else if (str == "AGGREGATE_SUM") {
    return EXPRESSION_TYPE_AGGREGATE_SUM;
  } else if (str == "AGGREGATE_MIN") {
    return EXPRESSION_TYPE_AGGREGATE_MIN;
  } else if (str == "AGGREGATE_MAX") {
    return EXPRESSION_TYPE_AGGREGATE_MAX;
  } else if (str == "AGGREGATE_AVG") {
    return EXPRESSION_TYPE_AGGREGATE_AVG;
  } else if (str == "AGGREGATE_WEIGHTED_AVG") {
    return EXPRESSION_TYPE_AGGREGATE_WEIGHTED_AVG;
  }

  return EXPRESSION_TYPE_INVALID;
}

//===--------------------------------------------------------------------===//
// Index Method Type - String Utilities
//===--------------------------------------------------------------------===//

std::string IndexTypeToString(IndexType type) {
  switch (type) {
    case INDEX_TYPE_INVALID: {
      return "INVALID";
    }
    case INDEX_TYPE_BTREE: {
      return "BTREE";
    }
  }
  return "INVALID";
}

IndexType StringToIndexType(std::string str) {
  if (str == "INVALID") {
    return INDEX_TYPE_INVALID;
  } else if (str == "BTREE") {
    return INDEX_TYPE_BTREE;
  }
  return INDEX_TYPE_INVALID;
}

//===--------------------------------------------------------------------===//
// Index  Type - String Utilities
//===--------------------------------------------------------------------===//

std::string IndexConstraintTypeToString(IndexConstraintType type) {
  switch (type) {
    case INDEX_CONSTRAINT_TYPE_INVALID: {
      return "INVALID";
    }
    case INDEX_CONSTRAINT_TYPE_DEFAULT: {
      return "NORMAL";
    }
    case INDEX_CONSTRAINT_TYPE_PRIMARY_KEY: {
      return "PRIMARY_KEY";
    }
    case INDEX_CONSTRAINT_TYPE_UNIQUE: {
      return "UNIQUE";
    }
  }
  return "INVALID";
}

IndexConstraintType StringToIndexConstraintType(std::string str) {
  if (str == "INVALID") {
    return INDEX_CONSTRAINT_TYPE_INVALID;
  } else if (str == "NORMAL") {
    return INDEX_CONSTRAINT_TYPE_DEFAULT;
  } else if (str == "PRIMARY_KEY") {
    return INDEX_CONSTRAINT_TYPE_PRIMARY_KEY;
  } else if (str == "UNIQUE") {
    return INDEX_CONSTRAINT_TYPE_UNIQUE;
  }
  return INDEX_CONSTRAINT_TYPE_INVALID;
}
//===--------------------------------------------------------------------===//
// Plan Node - String Utilities
//===--------------------------------------------------------------------===//

std::string PlanNodeTypeToString(PlanNodeType type) {
  switch (type) {
    case PLAN_NODE_TYPE_INVALID: {
      return "INVALID";
    }
    case PLAN_NODE_TYPE_ABSTRACT_SCAN: {
      return "ABSTRACT_SCAN";
    }
    case PLAN_NODE_TYPE_SEQSCAN: {
      return "SEQSCAN";
    }
    case PLAN_NODE_TYPE_INDEXSCAN: {
      return "INDEXSCAN";
    }
    case PLAN_NODE_TYPE_NESTLOOP: {
      return "NESTLOOP";
    }
    case PLAN_NODE_TYPE_NESTLOOPINDEX: {
      return "NESTLOOPINDEX";
    }
    case PLAN_NODE_TYPE_MERGEJOIN: {
      return "MERGEJOIN";
    }
    case PLAN_NODE_TYPE_UPDATE: {
      return "UPDATE";
    }
    case PLAN_NODE_TYPE_INSERT: {
      return "DELETE";
    }
    case PLAN_NODE_TYPE_DELETE: {
      return "DELETE";
    }
    case PLAN_NODE_TYPE_SEND: {
      return "SEND";
    }
    case PLAN_NODE_TYPE_RECEIVE: {
      return "RECEIVE";
    }
    case PLAN_NODE_TYPE_PRINT: {
      return "PRINT";
    }
    case PLAN_NODE_TYPE_AGGREGATE: {
      return "AGGREGATE";
    }
    case PLAN_NODE_TYPE_HASHAGGREGATE: {
      return "HASHAGGREGATE";
    }
    case PLAN_NODE_TYPE_UNION: {
      return "UNION";
    }
    case PLAN_NODE_TYPE_ORDERBY: {
      return "RECEIVE";
    }
    case PLAN_NODE_TYPE_PROJECTION: {
      return "PROJECTION";
    }
    case PLAN_NODE_TYPE_MATERIALIZE: {
      return "MATERIALIZE";
    }
    case PLAN_NODE_TYPE_LIMIT: {
      return "LIMIT";
    }
    case PLAN_NODE_TYPE_DISTINCT: {
      return "DISTINCT";
    }
    case PLAN_NODE_TYPE_SETOP: {
      return "SETOP";
    }
    case PLAN_NODE_TYPE_APPEND: {
      return "APPEND";
    }
    case PLAN_NODE_TYPE_RESULT: {
      return "RESULT";
    }
  }
  return "INVALID";
}

PlanNodeType StringToPlanNodeType(std::string str) {
  if (str == "INVALID") {
    return PLAN_NODE_TYPE_INVALID;
  } else if(str == "ABSTRACT_SCAN") {
    return PLAN_NODE_TYPE_ABSTRACT_SCAN;
  } else if (str == "SEQSCAN") {
    return PLAN_NODE_TYPE_SEQSCAN;
  } else if (str == "INDEXSCAN") {
    return PLAN_NODE_TYPE_INDEXSCAN;
  } else if (str == "NESTLOOP") {
    return PLAN_NODE_TYPE_NESTLOOP;
  } else if (str == "NESTLOOPINDEX") {
    return PLAN_NODE_TYPE_NESTLOOPINDEX;
  } else if (str == "UPDATE") {
    return PLAN_NODE_TYPE_UPDATE;
  } else if (str == "INSERT") {
    return PLAN_NODE_TYPE_INSERT;
  } else if (str == "DELETE") {
    return PLAN_NODE_TYPE_DELETE;
  } else if (str == "SEND") {
    return PLAN_NODE_TYPE_SEND;
  } else if (str == "RECEIVE") {
    return PLAN_NODE_TYPE_RECEIVE;
  } else if (str == "PRINT") {
    return PLAN_NODE_TYPE_PRINT;
  } else if (str == "AGGREGATE") {
    return PLAN_NODE_TYPE_AGGREGATE;
  } else if (str == "HASHAGGREGATE") {
    return PLAN_NODE_TYPE_HASHAGGREGATE;
  } else if (str == "UNION") {
    return PLAN_NODE_TYPE_UNION;
  } else if (str == "ORDERBY") {
    return PLAN_NODE_TYPE_ORDERBY;
  } else if (str == "PROJECTION") {
    return PLAN_NODE_TYPE_PROJECTION;
  } else if (str == "MATERIALIZE") {
    return PLAN_NODE_TYPE_MATERIALIZE;
  } else if (str == "LIMIT") {
    return PLAN_NODE_TYPE_LIMIT;
  } else if (str == "DISTINCT") {
    return PLAN_NODE_TYPE_DISTINCT;
  }
  return PLAN_NODE_TYPE_INVALID;
}
//===--------------------------------------------------------------------===//
// Constraint Type - String Utilities
//===--------------------------------------------------------------------===//

std::string ConstraintTypeToString(ConstraintType type) {
  switch (type) {
    case CONSTRAINT_TYPE_INVALID: {
      return "INVALID";
    }
    case CONSTRAINT_TYPE_NULL: {
      return "NULL";
    }
    case CONSTRAINT_TYPE_NOTNULL: {
      return "NOTNULL";
    }
    case CONSTRAINT_TYPE_DEFAULT: {
      return "DEFAULT";
    }
    case CONSTRAINT_TYPE_CHECK: {
      return "CHECK";
    }
    case CONSTRAINT_TYPE_PRIMARY: {
      return "PRIMARY_KEY";
    }
    case CONSTRAINT_TYPE_UNIQUE: {
      return "UNIQUE";
    }
    case CONSTRAINT_TYPE_FOREIGN: {
      return "FOREIGN_KEY";
    }
    case CONSTRAINT_TYPE_EXCLUSION: {
      return "EXCLUSION";
    }
  }
  return "INVALID";
}

ConstraintType StringToConstraintType(std::string str) {
  if (str == "INVALID") {
    return CONSTRAINT_TYPE_INVALID;
  } else if (str == "NULL") {
    return CONSTRAINT_TYPE_NULL;
  } else if (str == "NOTNULL") {
    return CONSTRAINT_TYPE_NOTNULL;
  } else if (str == "CHECK") {
    return CONSTRAINT_TYPE_DEFAULT;
  } else if (str == "DEFAULT") {
    return CONSTRAINT_TYPE_UNIQUE;
  } else if (str == "PRIMARY_KEY") {
    return CONSTRAINT_TYPE_CHECK;
  } else if (str == "UNIQUE") {
    return CONSTRAINT_TYPE_PRIMARY;
  } else if (str == "FOREIGN_KEY") {
    return CONSTRAINT_TYPE_FOREIGN;
  } else if (str == "EXCLUSION") {
    return CONSTRAINT_TYPE_EXCLUSION;
  }
  return CONSTRAINT_TYPE_INVALID;
}

ValueType PostgresValueTypeToPelotonValueType(
    PostgresValueType PostgresValType) {
  ValueType valueType = VALUE_TYPE_INVALID;

  switch (PostgresValType) {
    case POSTGRES_VALUE_TYPE_BOOLEAN:
      valueType = VALUE_TYPE_BOOLEAN;
      break;

    /* INTEGER */
    case POSTGRES_VALUE_TYPE_SMALLINT:
      valueType = VALUE_TYPE_SMALLINT;
      break;
    case POSTGRES_VALUE_TYPE_INTEGER:
      valueType = VALUE_TYPE_INTEGER;
      break;
    case POSTGRES_VALUE_TYPE_BIGINT:
      valueType = VALUE_TYPE_BIGINT;
      break;

    /* DOUBLE */
    case POSTGRES_VALUE_TYPE_DOUBLE:
      valueType = VALUE_TYPE_DOUBLE;
      break;

    /* CHAR */
    case POSTGRES_VALUE_TYPE_BPCHAR:
    case POSTGRES_VALUE_TYPE_BPCHAR2:
    case POSTGRES_VALUE_TYPE_VARCHAR:
    case POSTGRES_VALUE_TYPE_VARCHAR2:
    case POSTGRES_VALUE_TYPE_TEXT:
      valueType = VALUE_TYPE_VARCHAR;
      break;

    /* TIMESTAMPS */
    case POSTGRES_VALUE_TYPE_TIMESTAMPS:
    case POSTGRES_VALUE_TYPE_TIMESTAMPS2:
      valueType = VALUE_TYPE_TIMESTAMP;
      break;

    /* DECIMAL */
    case POSTGRES_VALUE_TYPE_DECIMAL:
      valueType = VALUE_TYPE_DECIMAL;
      break;

    /* INVALID VALUE TYPE */
    default:
      printf("INVALID VALUE TYPE : %d \n", PostgresValType);
      valueType = VALUE_TYPE_INVALID;
      break;
  }
  return valueType;
}

ConstraintType PostgresConstraintTypeToPelotonConstraintType(
    PostgresConstraintType PostgresConstrType) {
  ConstraintType constraintType = CONSTRAINT_TYPE_INVALID;

  switch (PostgresConstrType) {
    case POSTGRES_CONSTRAINT_NULL:
      constraintType = CONSTRAINT_TYPE_NULL;
      break;

    case POSTGRES_CONSTRAINT_NOTNULL:
      constraintType = CONSTRAINT_TYPE_NOTNULL;
      break;

    case POSTGRES_CONSTRAINT_DEFAULT:
      constraintType = CONSTRAINT_TYPE_DEFAULT;
      break;

    case POSTGRES_CONSTRAINT_CHECK:
      constraintType = CONSTRAINT_TYPE_CHECK;
      break;

    case POSTGRES_CONSTRAINT_PRIMARY:
      constraintType = CONSTRAINT_TYPE_PRIMARY;
      break;

    case POSTGRES_CONSTRAINT_UNIQUE:
      constraintType = CONSTRAINT_TYPE_UNIQUE;
      break;

    case POSTGRES_CONSTRAINT_FOREIGN:
      constraintType = CONSTRAINT_TYPE_FOREIGN;
      break;

    case POSTGRES_CONSTRAINT_EXCLUSION:
      constraintType = CONSTRAINT_TYPE_EXCLUSION;
      break;

    default:
      fprintf(stderr, "INVALID CONSTRAINT TYPE : %d \n", PostgresConstrType);
      break;
  }
  return constraintType;
}

}  // End peloton namespace
