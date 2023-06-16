#include <stdio.h>
#include <assert.h>

#include "pair-internal.h"
#include "pair.h"

typedef enum
{
  SRP_NG_2048,
  SRP_NG_3072,
  SRP_NG_CUSTOM
} SRP_NGType;

typedef struct
{
  int N_len;
  bnum N;
  bnum g;
} NGConstant;

struct NGHex
{
  int N_len;
  const char *n_hex;
  const char *g_hex;
};

static struct NGHex global_Ng_constants[] =
    {
        {/* 2048 */
         256,
         "AC6BDB41324A9A9BF166DE5E1389582FAF72B6651987EE07FC3192943DB56050A37329CBB4"
         "A099ED8193E0757767A13DD52312AB4B03310DCD7F48A9DA04FD50E8083969EDB767B0CF60"
         "95179A163AB3661A05FBD5FAAAE82918A9962F0B93B855F97993EC975EEAA80D740ADBF4FF"
         "747359D041D5C33EA71D281E446B14773BCA97B43A23FB801676BD207A436C6481F1D2B907"
         "8717461A5B9D32E688F87748544523B524B0D57D5EA77A2775D2ECFA032CFBDBF52FB37861"
         "60279004E57AE6AF874E7303CE53299CCC041C7BC308D82A5698F3A8D0C38271AE35F8E9DB"
         "FBB694B5C803D89F7AE435DE236D525F54759B65E372FCD68EF20FA7111F9E4AFF73",
         "2"},
        {/* 3072 */
         384,
         "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B"
         "139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485"
         "B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1F"
         "E649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23"
         "DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32"
         "905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF69558"
         "17183995497CEA956AE515D2261898FA051015728E5A8AAAC42DAD33170D04507A33A85521"
         "ABDF1CBA64ECFB850458DBEF0A8AEA71575D060C7DB3970F85A6E1E4C7ABF5AE8CDB0933D7"
         "1E8C94E04A25619DCEE3D2261AD2EE6BF12FFA06D98A0864D87602733EC86A64521F2B1817"
         "7B200CBBE117577A615D6C770988C0BAD946E208E24FA074E5AB3143DB5BFCE0FD108E4B82"
         "D120A93AD2CAFFFFFFFFFFFFFFFF",
         "5"},
        {0} /* null sentinel */
};

static void
free_ng(NGConstant *ng)
{
  if (!ng)
    return;

  bnum_free(ng->N);
  bnum_free(ng->g);
  free(ng);
}

static NGConstant *
new_ng(SRP_NGType ng_type, const char *n_hex, const char *g_hex)
{
  NGConstant *ng = calloc(1, sizeof(NGConstant));

  if (ng_type != SRP_NG_CUSTOM)
  {
    n_hex = global_Ng_constants[ng_type].n_hex;
    g_hex = global_Ng_constants[ng_type].g_hex;
  }

  bnum_hex2bn(ng->N, n_hex);
  bnum_hex2bn(ng->g, g_hex);

  ng->N_len = bnum_num_bytes(ng->N);

  assert(ng_type == SRP_NG_CUSTOM || ng->N_len == global_Ng_constants[ng_type].N_len);

  return ng;
}

int main(int argc, char *argv[])
{
  bnum k;
  NGConstant *ng;
  ng = new_ng(SRP_NG_3072, NULL, NULL);

  k = H_nn_pad(HASH_SHA512, ng->N, ng->g, ng->N_len);
  bnum_free(k);
  free_ng(ng);
  return 0;
}
