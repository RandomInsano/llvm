; RUN: llc < %s -march=avr | FileCheck %s

declare void @foo(i16*, i16*, i8*)

define void @test1(i16 %x) {
; CHECK-LABEL: test1:
; CHECK: out 61, r28
; SP copy
; CHECK-NEXT: in [[SPCOPY1:r[0-9]+]], 61
; CHECK-NEXT: in [[SPCOPY2:r[0-9]+]], 62
; allocate first dynalloca
; CHECK: in {{.*}}, 61
; CHECK: in {{.*}}, 62
; CHECK: sub
; CHECK: sbc
; CHECK: in r0, 63
; CHECK-NEXT: cli
; CHECK-NEXT: out 62, {{.*}}
; CHECK-NEXT: out 63, r0
; CHECK-NEXT: out 61, {{.*}}
; Test writes
; CHECK: std Z+12, {{.*}}
; CHECK: std Z+13, {{.*}}
; CHECK: std Z+7, {{.*}}
; CHECK-NOT: std
; Test SP restore
; CHECK: in r0, 63
; CHECK-NEXT: cli
; CHECK-NEXT: out 62, [[SPCOPY2]]
; CHECK-NEXT: out 63, r0
; CHECK-NEXT: out 61, [[SPCOPY1]]
  %a = alloca [8 x i16]
  %vla = alloca i16, i16 %x
  %add = shl nsw i16 %x, 1
  %vla1 = alloca i8, i16 %add
  %arrayidx = getelementptr inbounds [8 x i16]* %a, i16 0, i16 2
  store i16 3, i16* %arrayidx
  %arrayidx2 = getelementptr inbounds i16* %vla, i16 6
  store i16 4, i16* %arrayidx2
  %arrayidx3 = getelementptr inbounds i8* %vla1, i16 7
  store i8 44, i8* %arrayidx3
  %arraydecay = getelementptr inbounds [8 x i16]* %a, i16 0, i16 0
  call void @foo(i16* %arraydecay, i16* %vla, i8* %vla1)
  ret void
}

declare void @foo2(i16*, i64, i64, i64)

; Test that arguments are passed through pushes into the call instead of
; allocating the call frame space in the prologue. Also test that SP is restored
; after the call frame is restored and not before.
define void @dynalloca2(i16 %x) {
; CHECK-LABEL: dynalloca2:
; CHECK: in [[SPCOPY1:r[0-9]+]], 61
; CHECK: in [[SPCOPY2:r[0-9]+]], 62
; CHECK: push
; CHECK-NOT: st
; CHECK-NOT: std
; CHECK: call
; Call frame restore
; CHECK-NEXT: in r30, 61
; CHECK-NEXT: in r31, 62
; CHECK-NEXT: adiw r30, 8
; CHECK-NEXT: in r0, 63
; CHECK-NEXT: cli
; CHECK-NEXT: out 62, r31
; CHECK-NEXT: out 63, r0
; CHECK-NEXT: out 61, r30
; SP restore
; CHECK: in r0, 63
; CHECK-NEXT: cli
; CHECK-NEXT: out 62, r29
; CHECK-NEXT: out 63, r0
; CHECK-NEXT: out 61, r28
  %vla = alloca i16, i16 %x
  call void @foo2(i16* %vla, i64 0, i64 0, i64 0)
  ret void
}
