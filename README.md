
The Jack Programming language
=============================

![public domain](https://raw.githubusercontent.com/public-domain/jack/main/img/pd.png)


Public Domain JACK compiler
---------------------------

***

Abstract
--------
 
The [Jack programming language][1] is described in the book 
[The Elements of Computing Systems][8] and on the website 
[www.nand2tetris.org][7]. There is a [set of video][3] on Youtube talking about it.

This document is describing a Jack compiler written in Jack, itselfs bootstraped with a simple Python3 Jack compiler.

Status of this document
-----------------------

``` 
The author or authors of this Document dedicate any and all copyright interest 
in this Document to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and successors. 
We intend this dedication to be an overt act of relinquishment in perpetuity 
of all present and future rights to this Document under copyright law. 
```
> This is an early draft

> Your opinions are welcome!


Table of Contents
-----------------

1. [Introduction](#1-introduction)
     1. [Background](#11-background)
     2. [Differences](#12-differences)
2. [Hello World!](#2-hello-world)
3. [Grammar](#3-grammar)
     
***

## 1. Introduction

Elon Musk asks "is [life a video game][9]" ?

In a blues song (the roots of Hard Rock), AC/DC says "she's got [the Jack][4]" and "she's a dirty woman".

Could the roots of our Universe be coded in Jack programming language by our Creator ?

Jack is easy to learn but it is designed to be yet useful.


### 1.1 Background

![the number of](https://raw.githubusercontent.com/public-domain/jack/main/img/beast.jpg)

### 1.2 Differences

Diffrences from the original Jack specification are :

The range of decimal constant number and integer variable is unspecified. The size of the integer is at least the same size as the size of a memory adresse. (It could be from 8 to 512 bits).

The callback extension allows to call a callback method from a variable.

There is additional classes to allow interaction with the operating system (files and directories...).

## 2. Hello world

``` 
class Main {
	function void main()
	{
		do Output.printString("Hello World!");
		do Output.println();
		return;
	}
}
``` 

## 3. Grammar

``` 
keyword: one of
	class constructor function method callback field static var int char boolean void true false null this let do if else while return

symbol: one of
	{ } ( ) [ ] . , ; + - * / & | < > = ~

digit: one of
	0 1 2 3 4 5 6 7 8 9

letter: one of
	a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z

integerConstant:
	digit digit_opt

stringConstant:
	" unicodeExceptNewlineAndDoublequote_opt "

letterOrUnderscore:
	_
	letter

digitOrLetterOrUnderscore:
	letterOrUnderscore
	digit

identifier:
	letterOrUnderscore digitOrLetterOrUnderscore_opt

classDec:
	class className { classVarDecList_opt callbackDec_opt subroutineDecList_opt }

varNameList:
	varName
	varNameList, varName

classVarKind:
	static
	field

classVarDecList:
	classVarDec
	classVarDecList classVarDec

classVarDec:
	classVarKind type varNameList ;

type:
	int
	char
	boolean
	className

subroutineKind:
	constructor
	function
	method

subroutineType:
	type
	void

subroutineDecList:
	subroutineDec
	subroutineDecList subroutineDec

subroutineDec:
	subroutineKind subroutineType subroutineName ( parameterList_opt ) subroutineBody

callbackDec:
	callback int callback ( type varName , type varName ) subroutineBody

parameterList:
	type varName
	parameterList, type varName

subroutineBody:	
	{ varDecList_opt statements }

varDecList:
	varDec
	varDecList varDec

varDec:
	var type varNameList ;

varNameList:
	varName
	varNameList , varName

className:
	identifier

subroutineName:
	identifier

varName:
	identifier

statements:
	statement
	statements statement

statement:
	letStatement
	ifStatement
	whileStatement
	doSatement
	returnStatement

letStatement:
	let varName indexExpression_opt = expression ;

indexExpression:
	[ expression ]

ifStatement:
	if ( expression ) { statements } elseStatement_opt	

elseStatement:
	else { statements }

whileStatement:
	while ( expression ) { statements }

doStatement:
	do subroutineCall ;

returnStatement:
	return expression_opt;
	return callbackCall;

expression:
	term
	expression op term

term:
	integerConstant
	stringConstant
	keywordConstant
	varName indexExpression_opt
	subroutineCall
	( expression )
	unaryOp term

varOrClassName:
	className
	varName

subroutineCall:
	subroutineName ( expressionList_opt )
	varOrClassName . subroutineName ( expressionList_opt )

callbackCall:
	callback ( expression , expression )

expressionList:
	expression
	expressionList , expression

op: one of
	+ - * / & | < > =

unaryOp: one of
	- ~

keywordConstant:
	true
	false
	null
	this

``` 

***
[1]: https://www.nand2tetris.org/project09
[2]: https://www.csie.ntu.edu.tw/~cyy/courses/introCS/13fall/lectures/handouts/lec11_Jack.pdf
[3]: https://www.youtube.com/watch?v=KBTg0ju4rxM&list=PLrDd_kMiAuNmllp9vuPqCuttC1XL9VyVh
[4]: https://www.youtube.com/watch?v=kq_GSIw0X0w
[5]: https://commons.wikimedia.org/wiki/File:UN_emblem_blue.svg
[6]: https://tekeye.uk/playing_cards/svg-playing-cards 
[7]: https://www.nand2tetris.org/
[8]: https://www.amazon.com/Elements-Computing-Systems-Building-Principles/dp/0262640686/ref=ed_oe_p
[9]: https://www.youtube.com/watch?v=2KK_kzrJPS8 

***

