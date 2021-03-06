/*
 * Copyright 2010 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrTextContext_DEFINED
#define GrTextContext_DEFINED

#include "GrClip.h"
#include "GrGlyph.h"
#include "GrPaint.h"
#include "SkDeviceProperties.h"
#include "SkPostConfig.h"

class GrClip;
class GrContext;
class GrDrawContext;
class GrFontScaler;
class SkDrawFilter;
class SkTextBlob;

/*
 * This class wraps the state for a single text render
 */
class GrTextContext {
public:
    virtual ~GrTextContext();

    void drawText(GrRenderTarget* rt, const GrClip&,  const GrPaint&, const SkPaint&,
                  const SkMatrix& viewMatrix, const char text[], size_t byteLength, SkScalar x,
                  SkScalar y, const SkIRect& clipBounds);
    void drawPosText(GrRenderTarget* rt, const GrClip&, const GrPaint&, const SkPaint&,
                     const SkMatrix& viewMatrix,
                     const char text[], size_t byteLength,
                     const SkScalar pos[], int scalarsPerPosition,
                     const SkPoint& offset, const SkIRect& clipBounds);
    virtual void drawTextBlob(GrRenderTarget*, const GrClip&, const SkPaint&,
                              const SkMatrix& viewMatrix, const SkTextBlob*,
                              SkScalar x, SkScalar y,
                              SkDrawFilter*, const SkIRect& clipBounds);

    static bool ShouldDisableLCD(const SkPaint& paint);

protected:
    GrTextContext*                 fFallbackTextContext;
    GrContext*                     fContext;
    SkDeviceProperties             fDeviceProperties;

    SkAutoTUnref<GrRenderTarget>   fRenderTarget;
    GrClip                         fClip;
    SkIRect                        fClipRect;
    SkIRect                        fRegionClipBounds;
    GrPaint                        fPaint;
    SkPaint                        fSkPaint;

    GrTextContext(GrContext*, const SkDeviceProperties&);

    virtual bool canDraw(const GrRenderTarget*, const GrClip&, const GrPaint&,
                         const SkPaint&, const SkMatrix& viewMatrix) = 0;

    virtual void onDrawText(GrDrawContext*, GrRenderTarget*, const GrClip&,
                            const GrPaint&, const SkPaint&,
                            const SkMatrix& viewMatrix, const char text[], size_t byteLength,
                            SkScalar x, SkScalar y, const SkIRect& clipBounds) = 0;
    virtual void onDrawPosText(GrDrawContext*, GrRenderTarget*, const GrClip&,
                               const GrPaint&, const SkPaint&,
                               const SkMatrix& viewMatrix,
                               const char text[], size_t byteLength,
                               const SkScalar pos[], int scalarsPerPosition,
                               const SkPoint& offset, const SkIRect& clipBounds) = 0;

    void drawTextAsPath(GrDrawContext*, GrRenderTarget*, const GrClip& clip,
                        const SkPaint& origPaint, const SkMatrix& viewMatrix,
                        const char text[], size_t byteLength, SkScalar x, SkScalar y,
                        const SkIRect& clipBounds);
    void drawPosTextAsPath(GrDrawContext*, GrRenderTarget*, const GrClip& clip,
                           const SkPaint& origPaint, const SkMatrix& viewMatrix,
                           const char text[], size_t byteLength,
                           const SkScalar pos[], int scalarsPerPosition,
                           const SkPoint& offset, const SkIRect& clipBounds);

    void init(GrRenderTarget*, const GrClip&, const GrPaint&, const SkPaint&,
              const SkIRect& regionClipBounds);

    static GrFontScaler* GetGrFontScaler(SkGlyphCache* cache);
    // sets extent in stopVector and returns glyph count
    static int MeasureText(SkGlyphCache* cache, SkDrawCacheProc glyphCacheProc,
                           const char text[], size_t byteLength, SkVector* stopVector);
    static uint32_t FilterTextFlags(const SkDeviceProperties& devProps, const SkPaint& paint);

    friend class BitmapTextBatch;
};

#endif
