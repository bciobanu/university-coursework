Vue.prototype.axios = axios

var app = new Vue({
  el: '#app',

  data: {
    minPrice: 1,
    maxPrice: 10000,
    selected: "Any",
    books: [],
    input: {
      id: null,
      name: null,
      description: null,
      type: null,
      price: null
    }
  },

  created: function() {
    this.axios.get("/api/books")
              .then((response) => this.books = response.data)
  },

  methods: {
    commit: function() {
      this.input.id = books.length
      this.books.push(this.input)
      this.axios.post("/api/books", Object.assign({}, this.input))
    }
  },

  computed: {
    filterPrices: function() {
      return this.books.filter(
        book => book.price >= this.minPrice
          && book.price <= this.maxPrice
      )
    },

    getActiveBooks: function() {
      return this.filterPrices.filter(
        book => book.type == this.selected || this.selected == "Any"
      )
    },

    getActiveTypes: function() {
      return ["Any"].concat(
        this.filterPrices.map(book => book.type)
                         .filter((v, i, a) => a.indexOf(v) == i)
      )
    }
  }
})
